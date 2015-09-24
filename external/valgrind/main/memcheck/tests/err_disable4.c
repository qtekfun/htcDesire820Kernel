

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#include <semaphore.h>
#include <limits.h>    
#include "../include/valgrind.h"

char* block = NULL;
sem_t sem;

__attribute__((noinline)) void usechar ( char c )
{
   
   
   
   __asm__ __volatile__("" : : "r"(c) : "memory","cc");
}

__attribute__((noinline)) void err ( void )
{
   usechar( block[5] );
}

void* child_fn_1 ( void* arg )
{
   
   VALGRIND_DISABLE_ERROR_REPORTING;
   int r = sem_wait(&sem);  assert(!r);
   return NULL;
}

void* child_fn_2 ( void* arg )
{
   
   err();
   int r = sem_wait(&sem);  assert(!r);
   return NULL;
}

#define NTHREADS 498 

int main ( void )
{
  int r, i;
  pthread_t child[NTHREADS];

  block = malloc(10);
  free(block);

  
  fprintf(stderr, "\n-------- Letting %d threads exit "
                  "w/ errs disabled ------\n\n",
          NTHREADS);

  
  r = sem_init(&sem, 0, 0);  assert(!r);

  pthread_attr_t attr;
  r = pthread_attr_init(&attr); assert(!r);
  r = pthread_attr_setstacksize(&attr, PTHREAD_STACK_MIN);

  
  for (i = 0; i < NTHREADS; i++) {
     r = pthread_create(&child[i], &attr, child_fn_1, NULL);
     assert(!r);
  }

  
  for (i = 0; i < NTHREADS; i++) {
     r = sem_post(&sem);  assert(!r);
  }

  
  for (i = 0; i < NTHREADS; i++) {
     r = pthread_join(child[i], NULL);  assert(!r);
  }

  

  fprintf(stderr, "\n-------- Letting %d threads make an error "
                  "------\n\n",
          NTHREADS);
  

  
  for (i = 0; i < NTHREADS; i++) {
     r = pthread_create(&child[i], &attr, child_fn_2, NULL);
     assert(!r);
  }

  
  for (i = 0; i < NTHREADS; i++) {
     r = sem_post(&sem);  assert(!r);
  }

  
  for (i = 0; i < NTHREADS; i++) {
     r = pthread_join(child[i], NULL);  assert(!r);
  }

  
  
  int nerrors = VALGRIND_COUNT_ERRORS;
  fprintf(stderr, "\n-------- Got %d errors (expected %d ==> %s) ------\n\n", 
          nerrors, NTHREADS, nerrors == NTHREADS ? "PASS" : "FAIL" );

  return 0;
}
