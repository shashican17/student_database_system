#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Usage: %s <IP> <Port> <Input File> <Output File>\n", argv[0]);
        exit(1);
    }

    char *ip = argv[1];
    int port = atoi(argv[2]);
    char *input_file = argv[3];
    char *output_file = argv[4];

    if (port <= 0 || port > 65535) {
        fprintf(stderr, "Invalid port number. Must be between 1 and 65535\n");
        exit(1);
    }

    FILE *test_file = fopen(input_file, "r");
    if (!test_file) {
        perror("Input file error");
        exit(1);
    }
    fclose(test_file);

    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("Error in socket creation");
        exit(1);
    }

    memset(&server_addr, '\0', sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip);

    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error in connection");
        exit(1);
    }

    printf("Connected to server\n");

    ssize_t bytes_written = write(client_socket, input_file, strlen(input_file));
    if (bytes_written < 0) {
        perror("Error sending input filename");
        close(client_socket);
        exit(1);
    }
    sleep(1); 

    bytes_written = write(client_socket, output_file, strlen(output_file));
    if (bytes_written < 0) {
        perror("Error sending output filename");
        close(client_socket);
        exit(1);
    }

    ssize_t n = read(client_socket, buffer, BUFFER_SIZE - 1);
    if (n > 0) {
        buffer[n] = '\0';
        printf("Server response: %s\n", buffer);
    } else {
        printf("No response from server or error occurred.\n");
    }

    close(client_socket);

    return 0;
}