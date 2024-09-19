// Fadli Shidqi Firdaus
// 21120122140166
// Progjar D

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#define PORT 4444
#define BUFFER_SIZE 100
#define SERVER_IP "192.168.202.164"

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char msg[BUFFER_SIZE], server_msg[BUFFER_SIZE];
    ssize_t bytes_received;

    // Membuat socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Couldn't create socket");
        return 1;
    }
    printf("Socket created\n");

    // Menyiapkan alamat server
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // Menghubungkan ke server
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Couldn't connect");
        return 1;
    }
    printf("Connected to the server\n");

    while (1) {
        printf("Write message : ");
        if (fgets(msg, BUFFER_SIZE, stdin) == NULL) {
            break;
        }
        msg[strcspn(msg, "\n")] = 0;  // Menghapus newline dari input

        // Mengirim pesan ke server
        if (send(sockfd, msg, strlen(msg), 0) < 0) {
            perror("Error sending message");
            break;
        }

        // Menerima balasan dari server
        memset(server_msg, 0, sizeof(server_msg));
        bytes_received = recv(sockfd, server_msg, sizeof(server_msg) - 1, 0);
        if (bytes_received <= 0) {
            perror("Connection closed or error occurred");
            break;
        }

        server_msg[bytes_received] = '\0';
        printf("Server reply : %s\n", server_msg);

        if (strcmp(server_msg, "bye") == 0) {
            printf("Exiting...\n");
            break;
        }
    }

    close(sockfd);
    return 0;
}