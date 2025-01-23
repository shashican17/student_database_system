#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>
#include "database.h"
#include "student.h"
#include "course.h"
#include "sDB.h"


#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

typedef struct {
    sem_t mutex;
   
} SharedMemory;

void handle_client(int client_socket, SharedMemory* shm);
void sigchld_handler(int s);

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <IP> <Port>\n", argv[0]);
        exit(1);
    }

    char *ip = argv[1];
    int port = atoi(argv[2]);

    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;

    SharedMemory *shm = mmap(NULL, sizeof(SharedMemory), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (shm == MAP_FAILED) {
        perror("mmap failed");
        exit(1);
    }

    sem_init(&shm->mutex, 1, 1);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Error in socket creation");
        exit(1);
    }

    memset(&server_addr, '\0', sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip);

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error in binding");
        exit(1);
    }

    if (listen(server_socket, MAX_CLIENTS) == 0) {
        printf("Listening on %s:%d\n", ip, port);
    } else {
        perror("Error in listening");
        exit(1);
    }

    struct sigaction sa;
    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    while(1) {
        addr_size = sizeof(client_addr);
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &addr_size);
        if (client_socket < 0) {
            perror("Error in accepting");
            continue;
        }

        printf("Connection accepted from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        pid_t pid = fork();
        if (pid < 0) {
            perror("Error in fork");
            close(client_socket);
        } else if (pid == 0) {  
            close(server_socket);
            handle_client(client_socket, shm);
            exit(0);
        } 
        close(client_socket);
    }

    munmap(shm, sizeof(SharedMemory));

    return 0;
}

void handle_client(int client_socket, SharedMemory* shm) {
    char buffer[BUFFER_SIZE];
    char input_file[256], output_file[256];
    ssize_t n;

    memset(input_file, 0, sizeof(input_file));
    n = read(client_socket, input_file, sizeof(input_file) - 1);
    if (n <= 0) {
        fprintf(stderr, "Error receiving input file name from client\n");
        close(client_socket);
        return;
    }
    input_file[n] = '\0';
    fprintf(stdout, "Received request: Input file: %s\n", input_file);


    memset(output_file, 0, sizeof(output_file));
    n = read(client_socket, output_file, sizeof(output_file) - 1);
    if (n <= 0) {
        fprintf(stderr, "Error receiving output file name from client\n");
        close(client_socket);
        return;
    }
    output_file[n] = '\0';

    printf("Received request: Input file: %s, Output file: %s\n", input_file, output_file);

    sem_wait(&shm->mutex);

   StudentDatabase* db = createDatabase();
    if (parseInput(db, input_file) != 0) {
        snprintf(buffer, BUFFER_SIZE, "Error processing input file: %s", input_file);
        write(client_socket, buffer, strlen(buffer));
        fprintf(stderr, "Error processing input file: %s\n", input_file);
        freeDatabase(db);
        close(client_socket);
        return;
    }

    if (storeOutput(db, output_file) != 0) {
        snprintf(buffer, BUFFER_SIZE, "Error writing to output file: %s", output_file);
        write(client_socket, buffer, strlen(buffer));
        fprintf(stderr, "Error writing to output file: %s\n", output_file);
        freeDatabase(db);
        close(client_socket);
        return;
    }

    snprintf(buffer, BUFFER_SIZE, "Successfully processed input file: %s and wrote to output file: %s", 
            input_file, output_file);
    write(client_socket, buffer, strlen(buffer));
    fprintf(stdout, "Successfully processed input file: %s and wrote to output file: %s\n", input_file, output_file);

    freeDatabase(db);

    sem_post(&shm->mutex);

    close(client_socket);
}

void sigchld_handler(int s) {
    while(waitpid(-1, NULL, WNOHANG) > 0);
}
