#include<unistd.h>
#include<stdio.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<string.h>
#include<pthread.h>

#define PORT 8080

#define MAX_BUFFER_SIZE 1024

void *connection_handler(void *);

int main(int argc, char *argv[]) {
    int socket_d, socket_len, new_socket;
    socket_d = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_d == -1) {
        perror("socket creation failed.");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in client, server;

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    if (bind(socket_d, (struct sockaddr *) &server, sizeof(server)) < 0) {
        perror("bind failed.");
        exit(EXIT_FAILURE);
    }

    listen(socket_d, 3);

    int *temp_socket;
    socket_len = sizeof(struct sockaddr_in);
    while (new_socket = accept(socket_d, (struct sockaddr *) &client, (socklen_t*) &socket_len)) {
        pthread_t handler_t;
        
        temp_socket = malloc(1);
        *temp_socket = new_socket;

        if (pthread_create(&handler_t, NULL, connection_handler, (void *) temp_socket) < 0) {
            perror("thread creation failed.");
            exit(EXIT_FAILURE);
        }

        printf("[INFO] Handler assigned.\n");
    }

    if (new_socket < 0) {
        perror("accept failed.");
        exit(EXIT_FAILURE);
    }
}

char *strappend(char *s1, char *s2) {
    char *ret = malloc(strlen(s1) + strlen(s2) + 1);
    strcpy(ret, s1);
    strcat(ret, s2);

    return ret;
}


void *connection_handler(void *socket_d) {
    int socket = *(int *) socket_d;
    int valread, i;
    char buffer[MAX_BUFFER_SIZE];
    char *temp, ch;

    bzero(buffer, MAX_BUFFER_SIZE);
    temp = malloc(sizeof(char));

    int temp_sz = 1;

    while ((valread = read(socket, buffer, MAX_BUFFER_SIZE)) > 0) {
        i = 0;
        printf("[VAL_READ %d] %d bytes\n", socket, valread);

        while (i < valread) {
            if (buffer[i] == '\0') {
                temp[temp_sz - 1] = '\0';

                if (strcmp(temp, "END") == 0) {
                    printf("[INFO %d] Connection terminated by client.\n", socket);
                    free(temp);
                    free(socket_d);
                    return 0;
                }

                else {
                    printf("[COMM FR: %d] %s\n", socket, temp);
                    send(socket, temp, temp_sz, 0);
                }

                free(temp);
                temp = malloc(sizeof(char));
                temp_sz = 1;
            }

            else {
                temp[temp_sz - 1] = buffer[i];
                temp = realloc(temp, temp_sz + 1);
                temp_sz++;
            }

            i++;
        }
    }

    if (valread == 0) {
        printf("[INFO %d] Client disconnected.\n", socket);
        fflush(stdout);
    }

    else if (valread == -1) {
        perror("recv failed.");
    }

    free(socket_d);
    free(temp);
    return 0;
}
