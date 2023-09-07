# sisop-praktikum-fp-2023-BJ-U04

| Name                        | NRP        |
|-----------------------------|------------|
|Hanifi Abrar Setiawan        | 5025211066 |
|Faraihan Rafi Adityawarman   | 5025211074 |
|Vija Wildan Gita Prabawa     | 5025211261 |

## Authenticate Program

### Code
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char username[20];
    char password[20];
    int access_level;
} User;

int authenticateUser(User* users, int numUsers, char* username, char* password) {
    int i;
    for (i = 0; i < numUsers; i++) {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) {
            return users[i].access_level;
        }
    }
    return -1;  // -1 menunjukkan bahwa autentikasi gagal
}

void createUser(User* users, int* numUsers, char* username, char* password, int access_level) {
    if (access_level == 0) {  // Hanya root yang dapat membuat pengguna baru
        strcpy(users[*numUsers].username, username);
        strcpy(users[*numUsers].password, password);
        users[*numUsers].access_level = access_level;
        (*numUsers)++;
        printf("Pengguna berhasil ditambahkan.\n");
    } else {
        printf("Hanya root yang dapat membuat pengguna baru.\n");
    }
}

int main(int argc, char* argv[]) {
    // Inisialisasi pengguna
    User users[100];  // Misalnya, kita batasi maksimal 100 pengguna
    int numUsers = 1;  // Jumlah pengguna saat ini, dimulai dari 1 karena root sudah ada

    strcpy(users[0].username, "root");
    strcpy(users[0].password, "root123");
    users[0].access_level = 0;

    // Memeriksa argumen baris perintah
    if (argc != 5) {
        printf("Format: ./%s -u [username] -p [password]\n", argv[0]);
        return 1;
    }

    // Mendapatkan argumen yang diberikan
    char* username = argv[2];
    char* password = argv[4];

    // Memeriksa autentikasi pengguna
    int access_level = authenticateUser(users, numUsers, username, password);
    if (access_level == -1) {
        printf("Autentikasi gagal. Username atau password salah.\n");
        return 1;
    }

    // Memeriksa perintah yang diminta
    if (strcmp(argv[1], "-u") == 0 && strcmp(argv[3], "-p") == 0) {
        if (access_level == 0) {
            // Jika pengguna adalah root, berarti akses ke semua database
            printf("Akses diberikan ke semua database.\n");
        } else {
            // Jika pengguna bukan root, berarti hanya akses ke database yang sesuai dengan hak aksesnya
            printf("Akses diberikan ke database tertentu sesuai dengan hak akses.\n");
        }
    } else if (strcmp(argv[1], "CREATE") == 0 && strcmp(argv[2], "USER") == 0 &&
               strcmp(argv[4], "IDENTIFIED") == 0 && strcmp(argv[5], "BY") == 0) {
        if (access_level == 0) {
            // Hanya root yang dapat membuat pengguna baru
            createUser(users, &numUsers, argv[3], argv[6], 1);  // Misalnya, hak akses baru adalah 1
        } else {
            printf("Hanya root yang dapat membuat pengguna baru.\n");
        }
    } else {
        printf("Perintah tidak valid.\n");
    }

    return 0;
}
```

### Explanation
- The necessary header files, including stdio.h, stdlib.h, and string.h, are included.

- The code defines a User struct that represents a user with three fields: username, password, and access_level.

- The authenticateUser function takes an array of User objects, the number of users, a username, and a password as parameters. It iterates through the array and compares the provided username and password with each user's credentials. If a match is found, it returns the access level of the authenticated user. If no match is found, it returns -1 indicating authentication failure.

- The createUser function is responsible for creating a new user. It takes an array of User objects, a pointer to the number of users, a username, a password, and an access level as parameters. It checks if the access level is 0 (root level). If so, it copies the provided username, password, and access level to the next available position in the users array and increments the numUsers variable. If the access level is not 0, it prints an error message indicating that only the root user can create new users.

- The main function is the entry point of the program. It starts by initializing the users array with one user, the root user, having the username "root," password "root123," and access level 0.

- The program checks the command-line arguments provided to it. If the number of arguments is not 5, it prints a message indicating the correct format and exits the program.

- If the command-line arguments are valid, it extracts the provided username and password from the arguments.

- The program calls the authenticateUser function to check the provided username and password against the existing users. If authentication fails (the function returns -1), an error message is printed, and the program exits.

- If authentication is successful, the program checks the requested command by comparing the command-line arguments. If the command is "-u" (username) and "-p" (password), it further checks the user's access level. If the access level is 0 (root), it grants access to all databases. Otherwise, it grants access to specific databases based on the user's access level.

- If the command is "CREATE USER IDENTIFIED BY," the program checks if the user has root access. If so, it calls the createUser function to create a new user with the provided username, password, and access level of 1. If the user does not have root access, an error message is displayed.

- If the command does not match any of the valid options, an error message indicating an invalid command is printed.

- Finally, the program returns 0 to indicate successful execution.

## Database program

### Library

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

```

### DDL 

```c
while (1) {
        printf("SQL Command: ");
        fgets(input, sizeof(input), stdin);

        // Menghilangkan karakter newline pada akhir input
        input[strcspn(input, "\n")] = 0;

        // Mengekstrak perintah utama
        sscanf(input, "%s", command);

        // Mengeksekusi perintah SQL yang sesuai
        if (strcasecmp(command, "CREATE") == 0) {
            char objectType[100];
            sscanf(input, "%*s %s", objectType);

            if (strcasecmp(objectType, "DATABASE") == 0) {
                char dbName[100];
                sscanf(input, "%*s %*s %s", dbName);
                char createDBSQL[1000];
                snprintf(createDBSQL, sizeof(createDBSQL), "CREATE DATABASE %s;", dbName);
                rc = sqlite3_exec(db, createDBSQL, 0, 0, &errMsg);
                if (rc != SQLITE_OK) {
                    fprintf(stderr, "Failed making database: %s\n", errMsg);
                    sqlite3_free(errMsg);
                } else {
                    printf("Database '%s' Has Been Made\n", dbName);
                }
            } else if (strcasecmp(objectType, "TABLE") == 0) {
                char tableName[100];
                char tableColumns[1000];
                sscanf(input, "%*s %*s %s %[^\n]", tableName, tableColumns);
                char createTableSQL[1500];
                int size = snprintf(createTableSQL, sizeof(createTableSQL), "CREATE TABLE %s (%s);", tableName, tableColumns);
                if (size >= sizeof(createTableSQL)) {
                    fprintf(stderr, "Buffer Size is not enought\n");
                    return 1;
                }
                rc = sqlite3_exec(db, createTableSQL, 0, 0, &errMsg);
                if (rc != SQLITE_OK) {
                    fprintf(stderr, "Gagal membuat tabel: %s\n", errMsg);
                    sqlite3_free(errMsg);
                } else {
                    printf("Tabel '%s' berhasil dibuat\n", tableName);
                }
            }
        } else if (strcasecmp(command, "DROP") == 0) {
            char objectType[100];
            sscanf(input, "%*s %s", objectType);

            if (strcasecmp(objectType, "DATABASE") == 0) {
                char dbName[100];
                sscanf(input, "%*s %*s %s", dbName);
                char dropDBSQL[1000];
                snprintf(dropDBSQL, sizeof(dropDBSQL), "DROP DATABASE %s;", dbName);
                rc = sqlite3_exec(db, dropDBSQL, 0, 0, &errMsg);
                if (rc != SQLITE_OK) {
                    fprintf(stderr, "Failed to drop database: %s\n", errMsg);
                    sqlite3_free(errMsg);
                } else {
                    printf("Database '%s' succesfully dropped\n", dbName);
                }
            } else if (strcasecmp(objectType, "TABLE") == 0) {
                char tableName[100];
                sscanf(input, "%*s %*s %s", tableName);
                char dropTableSQL[1000];
                snprintf(dropTableSQL, sizeof(dropTableSQL), "DROP TABLE %s;", tableName);
                rc = sqlite3_exec(db, dropTableSQL, 0, 0, &errMsg);
                if (rc != SQLITE_OK) {
                    fprintf(stderr, "Failed to drop table: %s\n", errMsg);
                    sqlite3_free(errMsg);
                } else {
                    printf("Tabel '%s' succesfully dropped\n", tableName);
                }
            }

```

#### Explanation 
- The function is an infinite loop controlled by the condition while (1). This means the loop will continue executing until it is explicitly terminated.

- Inside the loop, the program prompts the user for an SQL command by printing "SQL Command: " using printf.

- The user's input is read using fgets and stored in the input variable. It ensures that the input does not exceed the size of the input buffer.

- The newline character at the end of the input is removed by replacing it with a null terminator. This is done using input[strcspn(input, "\n")] = 0;.

- The main command is extracted from the input using sscanf(input, "%s", command). The extracted command is stored in the command variable.

- The program checks the value of command to determine which SQL operation to execute.
 
- If the command is "CREATE", the program proceeds to check the object type to be created (either "DATABASE" or "TABLE") using sscanf(input, "%*s %s", objectType).

- If the object type is "DATABASE", the program extracts the database name from the input using sscanf(input, "%*s %*s %s", dbName) and creates an SQL command (CREATE DATABASE) with the provided database name.

- The SQL command is executed using sqlite3_exec, and the return code (rc) is checked to determine if the execution was successful.
 - If the execution failed, an error message is printed.
 - If the execution succeeded, a success message is printed.
 - If the object type is "TABLE", the program extracts the table name and columns from the input using sscanf(input, "%*s %*s %s %[^\n] ", tableName, tableColumns).

- The extracted table name and columns are used to create an SQL command (CREATE TABLE).
 - The SQL command is executed, and the return code is checked.
 - Error or success messages are printed accordingly.

-  If the command is "DROP", the program follows a similar pattern as the "CREATE" command to determine the object type ("DATABASE" or "TABLE") and execute the corresponding SQL command to drop the specified database or table.

- The loop continues, prompting for the next SQL command from the user, and the process repeats.

### DML 

```c
else if (strcasecmp(command, "SELECT") == 0) {
            rc = sqlite3_exec(db, input, callback, 0, &errMsg);
            if (rc != SQLITE_OK) {
                fprintf(stderr, "Failed to run command SELECT: %s\n", errMsg);
                sqlite3_free(errMsg);
            }
        } else if (strcasecmp(command, "INSERT") == 0 || strcasecmp(command, "UPDATE") == 0 || strcasecmp(command, "DELETE") == 0) {
            rc = sqlite3_exec(db, input, 0, 0, &errMsg);
            if (rc != SQLITE_OK) {
                fprintf(stderr, "Failed to run command SQL: %s\n", errMsg);
                sqlite3_free(errMsg);
            } else {
                printf("SQL command executed\n");
            }
        } else {
            printf("the commands are not valid\n");
        }

```

#### Explanation

1. The code checks if the command is "SELECT" using strcasecmp(command, "SELECT") == 0.

 - If it is a SELECT command, the program executes the input SQL command using sqlite3_exec(db, input, callback, 0, &errMsg).
 - The callback function is provided to handle the results of the SELECT query.
 - If the execution fails, an error message is printed.
 - The error message is freed using sqlite3_free(errMsg).
 - If the command is "INSERT", "UPDATE", or "DELETE", the code executes the input SQL command directly.

2. The input SQL command is executed using sqlite3_exec(db, input, 0, 0, &errMsg).
 - No callback function is provided since these commands typically do not return results.
 - If the execution fails, an error message is printed.
 - The error message is freed using sqlite3_free(errMsg).
 - If the execution succeeds, a success message is printed.

3. If the command is not any of the above valid commands, the code prints "the commands are not valid".


#### Containerization

```dockerfile
# Set the base image
FROM ubuntu:latest

# Set the working directory
WORKDIR /app

# COPY database.c /app
COPY database.c /app

# Install dependencies and set up the environment
RUN apt-get update && \
    apt-get install -y gcc

# Expose the port
# EXPOSE 8080

# Define the command to compile and run the C program
CMD gcc -o database database.c && ./storage
```

save this dockerfile, then run the command in terminal below:
```
// building the image
docker build -t storage-app .

// tagging the image
docker tag storage-app faraihanrafia/storage-app:ybba

// pushing the image
docker login // inserting credentials
docker push faraihanrafia/database:ybba
```

then after that we can see the tag in the web :  https://hub.docker.com/r/faraihanrafia/database.

```yml
version: '3'
services:
  databases:
    build:
      context: /home/pibe/SisopFP/
      dockerfile: dockerfile
    deploy:
      replicas: 5
```

the service are provided. inside the service, there's the directories and the file of dockerfile, then we deploy them with 5 replicas.

run the yml file with commands below in terminal

```cmd
docker-compose up -d
```
