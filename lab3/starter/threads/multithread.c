#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include "circular_queue.h"

int number_producers, total_consumed, total;
pthread_mutex_t queue_mux, consumed_mux;
sem_t items, spaces;
circular_queue buffer;

void* producer(void *arg){
  int* id = (int*) arg;

  while(*id < total){
    sem_wait(&spaces);
    pthread_mutex_lock(&queue_mux);
    enqueue(&buffer, *id);
    pthread_mutex_unlock(&queue_mux);
    sem_post(&items);
    *id += number_producers;
  }
  free(arg);
  pthread_exit(NULL);
}

void* consumer(void* arg){
  int* id = (int*) arg;
  int val;

  while(total_consumed < total){
    sem_wait(&items);
    if(total_consumed >= total){
        sem_post(&items);
        free(id);
        pthread_exit(0);
    }
    pthread_mutex_lock(&queue_mux);
    dequeue(&buffer, &val);
    pthread_mutex_unlock(&queue_mux);
    pthread_mutex_lock(&consumed_mux);
    total_consumed++;
    pthread_mutex_unlock(&consumed_mux);
    sem_post(&spaces);

    if (total_consumed >= total) sem_post(&items);
    int root = sqrt(val);
    if (root*root == val){
      printf("%d %d %d\n", *id, val, root);
    }
  }
  free(id);
  pthread_exit(NULL);
}

int main(int argc, const char* argv[]){
  int n, p, b, c;

  if (argc < 5){
    printf("Make sure you input 4 arguments\n");
    return -1;
  }

  n = atoi(argv[1]);
  b = atoi(argv[2]);
  p = atoi(argv[3]);
  c = atoi(argv[4]);
  number_producers = p;
  total = n;
  total_consumed = 0;

  struct timeval tv;
  double t1;
  double t2;

  newQueue(&buffer, b);
  sem_init(&items, 0, 0);
  sem_init(&spaces, 0, b);
  pthread_mutex_init(&queue_mux, NULL);
  pthread_mutex_init(&consumed_mux, NULL);
  pthread_t producers[number_producers];
  pthread_t consumers[c];

  // start producers and consumers and then join them
  gettimeofday(&tv, NULL);
	t1 = tv.tv_sec + tv.tv_usec/1000000.0;

  for (int i=0; i<number_producers; i++){
    int *pid = (int*) malloc(sizeof(int));
    *pid = i;
    pthread_create(&producers[i], NULL, producer, pid);
  }

  for (int i=0; i<c; i++){
    int *cid = (int*) malloc(sizeof(int));
    *cid = i;
    pthread_create(&consumers[i], NULL, consumer, cid);
  }

  for (int i=0; i<number_producers; i++){
    pthread_join(producers[i], NULL);
  }

  for (int i=0; i<c; i++){
    pthread_join(consumers[i], NULL);
  }

  gettimeofday(&tv, NULL);
  t2 = tv.tv_sec + tv.tv_usec/1000000.0;

  printf("System execution time: %.6lf seconds\n", t2-t1);

  destroy(&buffer);
  sem_destroy(&spaces);
  sem_destroy(&items);
  pthread_mutex_destroy(&queue_mux);
  pthread_mutex_destroy(&consumed_mux);

  pthread_exit(0);
}
