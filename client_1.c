#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>

#define PORT 8080

#define MAX_BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    int socket_d, ch, valread, i;
    int msg_size, num_reads;
    socket_d = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server;

    char message[MAX_BUFFER_SIZE];
    char *buffer;
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

    #if 1
    printf("1. Send data.\n2. End connection.\n");
    while (1) {
        printf("Enter choice: ");
        scanf("%d", &ch);

        switch(ch) {
            case 1:
                printf("Enter message: ");
                scanf(" %[^\n]s", message);

                #if 1
                msg_size = strlen(message) + 1;
                num_reads = ((strlen(message) + 1) / MAX_BUFFER_SIZE);

                for (int j = 0; j < num_reads; j++) {
                    send(socket_d, message, MAX_BUFFER_SIZE, 0);
                }

                send(socket_d, message, msg_size - (num_reads * MAX_BUFFER_SIZE), 0);

                buffer = (char *) malloc(msg_size);
                bzero(buffer, msg_size);

                for (int j = 0; j < num_reads + 1; j++) {
                    valread = recv(socket_d, message, MAX_BUFFER_SIZE, 0);
                    printf("[VAL_READ] %d bytes\n", valread);
                    
                    i = 0;
                    while (i < valread) {
                        buffer[(MAX_BUFFER_SIZE * j) + i] = message[i]; 
                        i++;
                    }
                }

                printf("[SERV] %s\n", buffer);
                #endif
                break;

            case 2:
                printf("Terminating connection...\n");
                send(socket_d, end_msg, strlen(end_msg) + 1, 0);
                return 0;

            default:
                printf("[ERR] Invalid choice.\n");
        }

    }
    #endif 

    #if 0
    end_msg = "aaaaaaaaaaaaa";
    // for (int i = 0; i < 100; i++)
        send(socket_d, end_msg, strlen(end_msg) + 1, 0);

    end_msg = "END";
    send(socket_d, end_msg, strlen(end_msg) + 1, 0);
    #endif

    return 0;
}