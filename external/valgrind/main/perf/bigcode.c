
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "tests/sys_mman.h"

#define FN_SIZE   996      
#define N_LOOPS   20000    
#define RATIO     4        

int f(int x, int y)
{
   int i;
   for (i = 0; i < 5000; i++) {
      switch (x % 8) {
       case 1:  y += 3;
       case 2:  y += x;
       case 3:  y *= 2;
       default: y--;
      }
   }
   return y;
}

int main(int argc, char* argv[])
{
   int h, i, sum1 = 0, sum2 = 0, sum3 = 0, sum4 = 0;
   int n_fns, n_reps;

   char* a = mmap(0, FN_SIZE * N_LOOPS, 
                     PROT_EXEC|PROT_WRITE, 
                     MAP_PRIVATE|MAP_ANONYMOUS, -1,0);
   assert(a != (char*)MAP_FAILED);

   if (argc <= 1) {
      
      n_fns  = N_LOOPS / RATIO;
      n_reps = RATIO;
      printf("mode 1: ");
   } else {
      
      n_fns  = N_LOOPS;
      n_reps = 1;
      printf("mode 1: ");
   }
   printf("%d copies of f(), %d reps\n", n_fns, n_reps);
   
   
   
   for (i = 0; i < n_fns; i++) {
      memcpy(&a[FN_SIZE*i], f, FN_SIZE);
   }
   
   for (h = 0; h < n_reps; h += 1) {
      for (i = 0; i < n_fns; i += 4) {
         int(*f1)(int,int) = (void*)&a[FN_SIZE*(i+0)];
         int(*f2)(int,int) = (void*)&a[FN_SIZE*(i+1)];
         int(*f3)(int,int) = (void*)&a[FN_SIZE*(i+2)];
         int(*f4)(int,int) = (void*)&a[FN_SIZE*(i+3)];
         sum1 += f1(i+0, n_fns-i+0);
         sum2 += f2(i+1, n_fns-i+1);
         sum3 += f3(i+2, n_fns-i+2);
         sum4 += f4(i+3, n_fns-i+3);
         if (i % 1000 == 0)
            printf(".");
      }
   }
   printf("result = %d\n", sum1 + sum2 + sum3 + sum4);
   return 0;
}
