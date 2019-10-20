#include "stdio.h"

void gen_random_num(int * data )
{
   static int fp= -1;

   if(-1 == fp )
   {
      fp = open("/dev/urandom",'r');
   }  

   read(fp,(char*)data,sizeof(int));
}


void main()
{
   int i =0;
   unsigned int temp = 0;

   for (i=0;i<10;i++)
   {
        gen_random_num(&temp); 
        printf("\n%d",temp);
   }

   printf("\n");
}
