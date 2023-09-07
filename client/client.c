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
