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
#define MAX_CONNECTIONS 5
#define BUFFER_SIZE 100

void handle_client(int client_sockfd) {
    char msg[BUFFER_SIZE];
    ssize_t bytes_received, bytes_sent;

    while (1) {
        bytes_received = recv(client_sockfd, msg, sizeof(msg) - 1, 0);
        if (bytes_received <= 0) {
            printf("Connection closed or error occurred\n");
            break;
        }

        msg[bytes_received] = '\0';
        printf("Received message: %s\n", msg);

        bytes_sent = send(client_sockfd, msg, bytes_received, 0);
        if (bytes_sent < 0) {
            perror("Error sending message");
            break;
        }

        if (strcmp(msg, "bye") == 0) {
            printf("Client requested to exit\n");
            break;
        }
    }

    close(client_sockfd);
    exit(0);
}

int main() {
    int sockfd, client_sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_size = sizeof(client_addr);

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
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // Binding socket
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Couldn't bind socket");
        return 1;
    }
    printf("Bound to port %d\n", PORT);

    // Mendengarkan koneksi
    if (listen(sockfd, MAX_CONNECTIONS) < 0) {
        perror("Couldn't listen to socket");
        return 1;
    }
    printf("Listening for connections (max backlog: %d)\n", MAX_CONNECTIONS);

    while (1) {
        // Menerima koneksi
        if ((client_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &client_addr_size)) < 0) {
            perror("Couldn't accept connection");
            continue;
        }
        printf("Accepted connection from %s\n", inet_ntoa(client_addr.sin_addr));

        // Membuat proses anak untuk menangani koneksi
        pid_t pid = fork();
        if (pid == 0) {
            close(sockfd);
            handle_client(client_sockfd);
        } else if (pid < 0) {
            perror("Couldn't create child process");
        }
        close(client_sockfd);
    }

    close(sockfd);
    return 0;
}