#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void gen_random_num(int * data,char close_file )
{
   static int fp= -1;

   if(-1 == fp )
   {
      fp = open("/dev/urandom",'r');
   }

   if( 1 == close_file )
   {
     close(fp);
     return ;
   }
   else
   {
       read(fp,(char*)data,sizeof(int));
   }
}

