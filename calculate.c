#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "log.h"

int logindex = 0;
int *logi = &logindex;
// mutex for logging functions required for thread-safe code
pthread_mutex_t tlock = PTHREAD_MUTEX_INITIALIZER;

int no_of_threads = 0;
#define ARRAYSIZE  1000000
int no_of_items = 0;

//global variables
long arr[ARRAYSIZE] = {};
int len_of_arr;
int all_sum = 0;
int* indexes;
pthread_t* ids;
//declare and initialize global mutex
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void init_arr() {
  srand(time(NULL));
  long i;
  for(i = 0; i < ARRAYSIZE; i++) {
    arr[i] = (long) (rand() % 10);
  }
}


void* doit(void *params) {
  // get from param the thread index
  int x = (int) params;
  int start = indexes[x];
  int sum = 0;
  int end = 0;
  int i;
  // find slice of the array -- from and to
  if (start + no_of_items > len_of_arr){
    end = len_of_arr - 1;
  }
  else{
    end = start + no_of_items - 1;
  }
  // display the message about which part of the array this threads does
  // log the message about which part of the array this threads does
  msg("Thread %d processing the array from %d to %d", x, start, end);
  Msg("Thread %d processing the array from %d to %d", x, start, end);
  // TO DO: sum up your portion of the array arr[]
   for(i = start; i <= end; i++){
    sum += arr[i];
  }
  // display the message about summation
  // log the message about summation
  msg("Thread %d summation is %d", x,sum);
  Msg("Thread %d summation is %d", x,sum);
  // lock mutex
  pthread_mutex_lock(&mutex);
  // update the global sum
  all_sum += sum;
  // unlock global mutex
  pthread_mutex_unlock(&mutex);
  // exit the thread
  pthread_exit(NULL);
}//end doit


// function main ----------------------------------------
int main(int argc,char *argv[]) {
  // local variables
   int i = 0;;
  // check args, argv if error, display message and terminate the program
  if (argc != 2){
    perror("wrong number of command line arguments\n");
    exit(0);
  }

  //set the number of threats to be dispatched
  //check it is between 2 and 10 inclusive, if not error and terminate
  if (atoi(argv[1]) < 2 || atoi(argv[1]) > 10 ){
     perror("wrong number of threads, must be 2 - 10\n");
     exit(0);
  }
  else{
    no_of_threads = atoi(argv[1]);
  }

  create_log("calculate.log");

  init_arr();

  pthread_mutex_init(&mutex,NULL);

  len_of_arr = sizeof(arr) / sizeof(arr[0]);
 
 
  //determine no_of_items each thread is supposed to sum up
  //create dynamically array to hold the index of each dispatched thread
  indexes = calloc(no_of_threads, sizeof(int));

  no_of_items = len_of_arr / no_of_threads;

  if(no_of_items * no_of_threads == len_of_arr){
    indexes[i] = 0;
    for(i=1;i < no_of_threads;i++){
      indexes[i] = indexes[i-1] + no_of_items;
    }
  }
  else{
    indexes[i] = 0;
    no_of_items ++;
  
    for(i = 1;i < no_of_threads;i++){
      indexes[i] = indexes[i-1] + no_of_items;
    }
  }
  //TO DO: create dynamically array to hold thread id of each dispatched thread
  ids = calloc(no_of_items, sizeof(pthread_t));

  for(i = 0;i<no_of_threads;i++){
    pthread_t tid;
    pthread_create(&tid,NULL,doit,(void *)(i));
    msg("dispatched thread %d with tid = %lu",i,tid);
    Msg("dispatched thread %d with tid = %lu",i,tid);
    ids[i] = tid;
  }
  //in a loop, dispatch each thread to execute doit() (see pthread_create())
  //and display and log message "dispatched thread XXX with tid = YYY"
  for(i = 0;i<no_of_threads;i++){
     pthread_t tid = ids[i];
     pthread_join(tid,NULL);
  }

  msg("Overall sum = %d",all_sum);

  //sum up the array arr[]
  int sum_check = 0;
  for(i=0;i<len_of_arr;i++){
    sum_check += arr[i];
  }
  //and display the message "Check sum = XXX"
  msg("Check sum = %d",sum_check);

  free(indexes);
  free(ids);

  return 0;
}//end main
