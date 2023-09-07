#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

// Callback function untuk menampilkan hasil SELECT
int callback(void *data, int argc, char **argv, char **azColName) {
    for (int i = 0; i < argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

int main() {
    sqlite3 *db;
    char *errMsg;
    int rc;

    char input[1000];
    char command[1000];

    // Membuka koneksi ke database atau membuat database baru jika belum ada
    rc = sqlite3_open("mydatabase.db", &db);
    if (rc) {
        fprintf(stderr, "cannot access the database: %s\n", sqlite3_errmsg(db));
        return 1;
    } else {
        printf("connected to the database\n");
    }

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
        } else if (strcasecmp(command, "SELECT") == 0) {
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
    }

    // Menutup koneksi ke database
    sqlite3_close(db);

    return 0;
}
