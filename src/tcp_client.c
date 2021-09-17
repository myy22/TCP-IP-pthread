#include "tcp_client.h"
#include "message.h"

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <stdio.h>

typedef struct sockaddr_in SA_IN;
typedef struct sockaddr SA;

void handle_connection(int server_socket)
{
    int keepAlive = 1;
    Message sent, received;
    char buff[MESSAGE_SIZE];
    int n;

    while(keepAlive)
    {
        bzero(buff, sizeof(buff));
        printf("Enter message to server: ");
        n = 0;
        while((buff[n++] = getchar()) != '\n');
        strcpy(sent.contents, buff);
        sent.keepAlive = 1;
        if((strncmp(buff, "exit", 4)) == 0)
            sent.keepAlive = 0;

        //Send message to server
        write(server_socket, &sent, sizeof(sent));

        //Read response from server
        read(server_socket, &received, sizeof(received));
        if(!received.keepAlive)
        {
            printf("Client Exit ...");
            keepAlive = 0;
            sent.keepAlive = 0;
            write(server_socket, &sent, sizeof(sent));
            continue;
        }
        printf("From Server : %s\n", received.contents);
    }
}

int main()
{
    //Create socket
    int server_socket = -1;
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    SA_IN addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    if(connect(server_socket, (SA*) &addr, sizeof(addr)) == -1)
        printf("Error making connection with remote socket\n");

    Message welcome;
    read(server_socket, &welcome, sizeof(welcome));
    printf("The server sent: %s\n", welcome.contents);

    handle_connection(server_socket);

    close(server_socket);
    return 0;
}