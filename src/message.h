#ifndef MESSAGE_H
#define MESSAGE_H

#define MESSAGE_SIZE 128
typedef struct message Message;

struct message{
    int keepAlive;
    char contents[MESSAGE_SIZE];
};

#endif //MESSAGE_H
