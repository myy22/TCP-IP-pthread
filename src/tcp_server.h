#ifndef TCP_SERVER_H
#define TCP_SERVER_H

//#include <pthread.h>

//#ifndef TCP_WORKERS
//#define TCP_WORKERS 8
//#endif

#define PORT 8080

typedef struct tcpserver TCP_Server;
typedef struct request Request;

//struct tcpserver
//{
//    int socket;
//
//    uint8_t keep_alive;
//    uint8_t is_alive;
//    uint8_t worker_n;
//    pthread_t* worker_threads;
//
//    pthread_mutex_t lock;
//    pthread_cond_t condition;
//};

#endif //TCP_SERVER_H
