//Jacob Silveria and Jose Castillo
//CST-315
//Assignment 2: Monitors and Semaphores
//2/13/2022
/*
This is the reader-writer problem dealt with through the use of monitors

To compile:
g++ -o gfg -pthread readwriteMon.cpp
To run:
./gfg
*/


//includes 
#include <iostream>
#include <pthread.h>
#include <unistd.h>
using namespace std;
 
//initilization
class monitor {
private:
//the number of readers
int rdrsc;
//the number of writers
int writc; 
//the count of readers that are waiting
int rdrsw;
//the count of writers that are waiting
int writw;

//a pthread condition variable that checks if reading is possible
pthread_cond_t canrd;
//a pthread condition varaible that checks if writing is possible
pthread_cond_t canwrt;

pthread_mutex_t condlock;
public:
    monitor() //the monitor class
    {
        //setting the readers, writers, readers waiting, and writers waiting to 0
        rdrsc = 0;
        writc = 0;
        rdrsw = 0;
        writw = 0;
 
        pthread_cond_init(&canrd, NULL);
        pthread_cond_init(&canwrt, NULL);
        pthread_mutex_init(&condlock, NULL);
    }
    //the mutex in this program ensures that no thread can change the data
    
    //for reader
    void startrd(int i) //start read
    {
    pthread_mutex_lock(&condlock);
    //if there are any current writers or writers in waiting
    if (writc == 1 || writw > 0) {
       rdrsw++; //increment for readers in wait
       pthread_cond_wait(&canrd, &condlock); //reader stopped
       rdrsw--;
    }
      //reader reads in resource
      rdrsc++;
      cout << "the reader " << i << " is reading\n"; //print statement for reader
      pthread_mutex_unlock(&condlock);
      pthread_cond_broadcast(&canrd);
    }
    void stoprd(int i) //end of reader, if there are none left than the writer can enter the monitor
    {
    pthread_mutex_lock(&condlock);
      if (--rdrsc == 0)
          pthread_cond_signal(&canwrt);
    pthread_mutex_unlock(&condlock);
    }
 

    // for writer
    void startwrt(int i) //start write
    {
    pthread_mutex_lock(&condlock);
    //the writer will be able to enter when there is no other writer or readers that are waiting
    if (writc == 1 || rdrsc > 0) {
       ++writw; //increment for writers
       pthread_cond_wait(&canwrt, &condlock);
       --writw;
    }
    writc = 1;
    cout << "the writer " << i << " is writing\n"; //print statment for writer
    pthread_mutex_unlock(&condlock);
    }
    void stopwrt(int i) //end of writer, checks if there are any readers that are waiting, if there are than the threads are unlocked
    {
    pthread_mutex_lock(&condlock);
    writc = 0;
    // if any readers are waiting, threads are unblocked
    if (rdrsw > 0)
        pthread_cond_signal(&canrd);
    else
        pthread_cond_signal(&canwrt);

    pthread_mutex_unlock(&condlock);
    }
}
 
//monitor class
mon;
void* rd(void* id) //for reader
{
int c = 0;
int i = *(int*)id;
//reader attempting to read for 10 times
while (c < 10) {
  usleep(1);
  mon.startrd(i);
  mon.stoprd(i);
  c++;
  }
}
 
void* wrt(void* id) //for writer
{
int c = 0;
int i = *(int*)id;
//writer attempting to write for 10 times
while (c < 10) {
  usleep(1);
  mon.startwrt(i);
  mon.stopwrt(i);
  c++;
  }
}


//main
int main()
{
pthread_t r[10], w[10];
int id[10];
for (int i = 0; i < 10; i++) {
id[i] = i;
//thread creation that will perform the reader function
pthread_create(&r[i], NULL, &rd, &id[i]);
//threads to perfrom writer function
pthread_create(&w[i], NULL, &wrt, &id[i]);
 }
for (int i = 0; i < 10; i++) {
    pthread_join(r[i], NULL);
 }
for (int i = 0; i < 10; i++) {
    pthread_join(w[i], NULL);
  }
}
