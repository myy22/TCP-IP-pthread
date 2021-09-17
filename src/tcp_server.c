#include <stdio.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "tcp_server.h"
#include "message.h"

#define SOCKETERROR -1
#define SERVER_BACKLOG 1

typedef struct sockaddr_in SA_IN;
typedef struct sockaddr SA;

int check(int exp, const char* msg)
{
    if (exp == SOCKETERROR)
    {
        perror(msg);
        exit(1);
    }
    return exp;
}

void handle_connection(int client_socket)
{
    Message sent, received;
    char buff[MESSAGE_SIZE];
    int n;

    while(1)
    {
        //Read message from the client
        read(client_socket, &received, sizeof(received));
        if(!received.keepAlive)
        {
            printf("Server Exit ...");
            sent.keepAlive = 0;
            write(client_socket, &sent, sizeof(sent));
            break;
        }
        printf("From client: %s\n", received.contents);

        printf("Enter message to client: ");
        bzero(buff, sizeof(buff));
        n = 0;
        while((buff[n++] = getchar()) != '\n');
        strcpy(sent.contents, buff);
        if((strncmp(buff, "exit", 4)) == 0)
            sent.keepAlive = 0;
        write(client_socket, &sent, sizeof(sent));
    }
}

int main(int argc, char** argv)
{
    int server_socket, client_socket;
    SA_IN server_addr;

    check((server_socket = socket(AF_INET, SOCK_STREAM, 0)), "Socket creation failed");

    //define the server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    check(bind(server_socket, (SA*) &server_addr, sizeof(server_addr)), "Binding failed");
    check(listen(server_socket, SERVER_BACKLOG), "Listening failed");

    //Handle the initial connection of the client and send a message
    Message welcome;
    strcpy(welcome.contents, "You have successfully connected to the server\n");
    welcome.keepAlive = 1;
    client_socket = accept(server_socket, NULL, NULL);
    send(client_socket, &welcome, sizeof(welcome), 0);

    handle_connection(client_socket);

    close(server_socket);

    return 0;
}
