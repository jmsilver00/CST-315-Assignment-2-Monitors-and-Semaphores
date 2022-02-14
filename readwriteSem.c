//Jacob Silveira and Jose Castillo
//CST-315
//Assignment 2: Monitors and Semaphores
//2/13/2022
/*
This is a reader-writer problem dealt with using semaphores and mutex
Out of the variations this is the first readers-writers problem, which requires that no reader is kept in waiting, unless a writer has gotten access to use the shared resource/object

To compile:
gcc readwriteSem.c -pthread
To run:
./a.out
*/

//includes
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>

//initilization
//here there will be a binary semaphore that can be used for signalling and mutual exclusion
sem_t writ;
pthread_mutex_t mutex; //mutex gives mutual exclusion when readers count is modified
int count = 1; 
int rdrs = 0; //tracking the total readers

//writer
void *writer(void *wno)
{   
  sem_wait(&writ); //write operation carried out
  count = count*2; //multiply value
  printf("Writer %d changed the count to %d\n",(*((int *)wno)),count);
  sem_post(&writ);

}

//reader
void *reader(void *rno)
{   
  //the reader needs to get the lock before it modifys the reader count
  pthread_mutex_lock(&mutex);
  rdrs++;
  if(rdrs == 1) {
    sem_wait(&writ); //if the reader is the first one the writer will be blocked 
  }
  //read operation is carried out
  pthread_mutex_unlock(&mutex);
  printf("Reader %d: changed the count to %d\n",*((int *)rno),count);
  //reader need to acquire lock again before chagning the reader count
  pthread_mutex_lock(&mutex);
  rdrs--;
  //if the reader is the last one the writer will be woken up
  if(rdrs == 0) {
      sem_post(&writ);
  }
  pthread_mutex_unlock(&mutex);
}

//main
int main()
{   
  pthread_t read[10],write[10];
  pthread_mutex_init(&mutex, NULL);
  sem_init(&writ,0,1);
  int a[10] = {1,2,3,4,5,6,7,8,9,10}; //array for numbering reader and writer 
  for(int i = 0; i < 10; i++) {
      pthread_create(&read[i], NULL, (void *)reader, (void *)&a[i]);
  }
  for(int i = 0; i < 10; i++) {
      pthread_create(&write[i], NULL, (void *)writer, (void *)&a[i]);
  }
  for(int i = 0; i < 10; i++) {
      pthread_join(read[i], NULL);
  }
  for(int i = 0; i < 10; i++) {
      pthread_join(write[i], NULL);
  }
  pthread_mutex_destroy(&mutex);
  sem_destroy(&writ); //destory semaphores
  return 0; 
}
