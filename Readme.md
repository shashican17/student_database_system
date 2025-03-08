# 📡 Socket-Based Student Database Management System

## 📝 Project Overview
This project implements a **Socket-Based Student Database Management System** using **C**. It facilitates communication between a **client** and a **server** to manage student records, courses, and database operations efficiently.

## 📂 Directory Structure
```
final with socket/
│-- client.c            # Client-side application
│-- server.c            # Server-side application
│-- main.c              # Entry point of the project
│-- database.c, database.h # Handles database operations
│-- course.c, course.h  # Manages course-related functions
│-- student.c, student.h # Student information handling
│-- shared_memory.h     # Shared memory-based communication (if used)
│-- sDB.h               # Header file for student database
│-- input.txt           # Sample input data
│-- makefile            # Compilation script
│-- Readme.md           # Project documentation (You're reading this!)
│-- 2127.out            # Compiled binary (if applicable)
```

## 🛠️ Features
✅ **Client-Server Architecture** – Uses sockets for network communication.  
✅ **Database Operations** – Supports adding, deleting, and updating student records.  
✅ **Course Management** – Allows enrolling students in courses and retrieving course details.
✅ **Makefile Support** – Easily compile and run the project using `make`.

## 🚀 Getting Started
### Prerequisites
Ensure you have the following installed:
- GCC Compiler (`gcc`)
- Make (`make`)
- Linux-based OS (recommended) or WSL (Windows Subsystem for Linux)

### 🔧 Compilation
To compile the project, run:
```sh
make
```
This will generate the necessary executable files.

### ▶️ Running the Project
1. **Start the Server**
   ```sh
   ./server
   ```
2. **Run the Client**
   ```sh
   ./client
   ```
3. Follow on-screen prompts to interact with the database.

## 📜 Code Explanation
### **Client (`client.c`)**
- Establishes a connection to the server.
- Sends user commands to the server.
- Receives and displays responses from the server.

### **Server (`server.c`)**
- Listens for incoming client connections.
- Handles database requests and processes client queries.
- Uses `fork()` for multi-client handling (if implemented).

### **Database (`database.c/h`)**
- Implements functions to manage student records.
- Provides search, insert, delete, and update operations.

### **Student & Course Modules (`student.c/h`, `course.c/h`)**
- Structures and functions to store and retrieve student/course information.

### **Shared Memory (`shared_memory.h`)**
- Enables inter-process communication (if implemented).

