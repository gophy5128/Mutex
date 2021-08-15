#include <stdio.h>
#include <cstdio>
#include <pthread.h>
#include <unistd.h>
#include <vector>
#include<time.h>
#include<stdlib.h>
#include <sys/wait.h>

#define BufferSize 1000

typedef struct Data{
    int id;
    int seconds;
    int status;
    int data;
    RWZONE* buffer;
}Data;
typedef struct RWZONE{
    int arr[1000];
    int wnext;
    int rnext;
    int status;
}RWZONE;

void ProducerFunction(void* arg){
    Data* write = (Data*)arg;
    int s = (rand()%10+1)*100000;
    while(true){
        usleep(s);
        while(write->buffer->status != write->id){
            if(write->buffer->status == 0&&write->buffer->wnext-write->buffer->rnext<1000)write->buffer->status = write->id;
        }
        write->seconds = time((time_t*)NULL);
        write->data = (rand() %10);
        printf("Producer %d write data: time is %d, data is %d", write->id, write->seconds. write->data);//write->id,not *write->id
        write->buffer->arr[write->buffer->wnext]= write->data;
        write->buffer->wnext++;
    }
}


void CustomerFunction(void* arg){
    Data* reader = (Data*)arg;
    int s = (rand()%10+1)*100000;
    while(true){
        usleep(s);
        while(reader->buffer->status != -reader->id){
            if(reader->buffer->status == 0&&reader->buffer->rnext<=reader->buffer->wnext)reader->buffer->status = -reader->id;
        }
        while(reader->buffer->rnext == reader->buffer->wnext);
        printf("Customer %d reads data: time is %d, data is %d\n",reader->id,s,reader->buffer->arr[reader->buffer->rnext]);
        reader->buffer->rnext ++;
        reader->buffer->status = 0;
    }
}

int main()
{
    RWZONE* buffer;
    buffer->wnext = 0;
    buffer->rnext = 0;
    buffer->status = -99;
    for(int i=0; i<2; i++)
    {
        Data* producer = new Data;
        producer->id = i+1;
        producer->buffer = buffer;
        pthread_t t;
        pthread_create(&t, NULL, ProduceFunction, (void*) producer);
        pthread_join(t, NULL);
    }
    for(int i=0; i<3; i++)
    {
        Data* customer = new Data;
        customer->id = i+1;
        customer->buffer = buffer;
        pthread_t t;
        pthread_create(&t, NULL, CustomerFunction, (void*) customer);
        pthread_join(t, NULL);
    }
}
