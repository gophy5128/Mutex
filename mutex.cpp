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
    int data;
}Data;

Data Buffer[1000];

void ProducerFunction(void* arg)
{
    Data* write = (Data*)arg;
    int s = (rand()%10+1)*100000;
    for (int i = 0; i < 5; i++){
        usleep(s);
        write->seconds = time((time_t*)NULL);
        write->data = (rand() %10);
        printf("Producer %d write data: time is %d, data is %d", *write->id, *write->seconds. *write->data);
    }


void CustomerFunction(void* arg)
{

}

int main()
{
    for(int i=0; i<2; i++)
    {
        Data* producer;
        producer->id = i+1;
        pthread_t t;
        pthread_create(&t, NULL, ProduceFunction, (void*) producer);
        pthread_join(t, NULL);
    }
    for(int i=0; i<3; i++)
    {
        Data* customer;
        customer->id = i+1;
        pthread_t t;
        pthread_create(&t, NULL, CustomerFunction, (void*) customer);
        pthread_join(t, NULL);
    }
}
