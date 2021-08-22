#include <stdio.h>
#include <cstdio>
#include <pthread.h>
#include <unistd.h>
#include <vector>
#include<time.h>
#include <stdlib.h>
#include <sys/wait.h>

#define BufferSize 1000
typedef struct trans{
    time_t time;
    int info;
}trans;
typedef struct RWZONE{
    trans* arr[1000];
    int wnext = 0;
}RWZONE;
typedef struct Data{
    int id;
    int next;
    pthread_mutex_t mutex;
    trans* data;
    RWZONE* buffer;
}Data;


void ProducerFunction(void* arg){
    Data* write = (Data*)arg;
    int s = (rand()%10+1)*100000;
    while(true){
        usleep(s);
        write->data->info = (rand() %10);
        write->data->time = time(0);
        pthread_mutex_lock(&write->mutex);
        write->buffer->arr[write->buffer->wnext]= write->data;
        write->buffer->wnext++;
        pthread_mutex_unlock(&write->mutex);
        printf("Producer %d write data: time is %lld, data is %d", write->id, (long long)write->data->time, write->data->info);//write->id,not *write->id
    }
}


void CustomerFunction(void* arg){
    Data* reader = (Data*)arg;
    int s = (rand()%10+1)*100000;
    while(true){
        usleep(s);///  ///
        printf("Customer %d reads data: time is %d, data is %d\n",reader->id,(int)reader->buffer->arr[reader->next]->time,reader->buffer->arr[reader->next]->info);
        reader->next++;
    }
}

int main(){
    pthread_mutex_t wmutex = PTHREAD_MUTEX_INITIALIZER;
    RWZONE* buffer = (RWZONE*)malloc(sizeof(RWZONE));
    buffer->wnext = 0;
    int threadids[5];
    for(int i=0; i<2; i++){
        Data* producer = (Data*)malloc(sizeof(Data));
        producer->id = i+1;
        producer->mutex = wmutex;
        producer->buffer = buffer;
        producer->data = (trans*)malloc(sizeof(trans));
        pthread_t t;
        pthread_create(&t, NULL, (void*)ProducerFunction, (void*) producer);
        threadids[i] = t;
    }
    for(int i=0; i<3; i++){
        Data* customer = (Data*)malloc(sizeof(Data));
        customer->id = i+1;
        customer->next = 0;
        customer->buffer = buffer;
        pthread_t t;
        pthread_create(&t, NULL, (void*)CustomerFunction, (void*) customer);
        threadids[i+2] = t;
    }
    for(int i =0;i<5;i++){
        pthread_join(threadids[i],nullptr);
    }
}

/*changes:
    1. change to mutex
    2. change buffer(int)into struct
*/

