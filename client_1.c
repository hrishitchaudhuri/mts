#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>

#define PORT 8080

int main(int argc, char *argv[]) {
    int socket_d, ch;
    socket_d = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server;

    char message[1024];
    char *end_msg = "END";

    socket_d = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_d < 0) {
        printf("Socket creation failed.\n");
        return -1;
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &server.sin_addr) <= 0) {
        printf("Invalid address.\n");
        return -1;
    }

    if (connect(socket_d, (struct sockaddr *) &server, sizeof(server)) < 0) {
        printf("Connection failed.\n");
        return -1;
    }

    #if 0
    printf("1. Send data.\n2. End connection.\n");
    while (1) {
        printf("Enter choice: ");
        scanf("%d", &ch);

        switch(ch) {
            case 1:
                printf("Enter message: ");
                scanf(" %[^\n]s", message);
                send(socket_d, message, strlen(message), 0);
                break;

            case 2:
                printf("Terminating connection...\n");
                send(socket_d, end_msg, strlen(end_msg), 0);
                return 0;

            default:
                printf("[ERR] Invalid choice.\n");
        }
    }
    #endif 

    #if 1
    end_msg = "aaaaaaaaaaaaa";
    // for (int i = 0; i < 100; i++)
        send(socket_d, end_msg, strlen(end_msg) + 1, 0);

    end_msg = "END";
    send(socket_d, end_msg, strlen(end_msg) + 1, 0);
    #endif

    return 0;
}