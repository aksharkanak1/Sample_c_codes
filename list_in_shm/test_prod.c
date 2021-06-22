#include "list_in_shm.h"
#include <string.h>
#define SHM_FILE  "/root/c_code/system-v-ipc/name"
#define SEM_FILE  "/sem_1"
#define ID 0xdeadbeef
#define MAX_NODES 1000
#define STR_MAX_SIZE

typedef struct data 
{
    uint32_t p_off ;
    uint32_t n_off ;
     data;
}data_t;

int main(int argc, char * argv[])
{
    list_in_shm_handle_t h = {0,};
    data_t * temp= NULL ;
    uint8_t *temp2=NULL ;
    int i =0;
    if( 2 != argc )
      exit(-1);

    if (strcmp(argv[1],"proc")== 0)
    {
         list_in_shm_init(&h,SHM_FILE,ID,sizeof(data_t),MAX_NODES,SEM_FILE,1);
#if 1
         for(i=0;i<MAX_NODES;i++)
         {
            temp=get_node_from_shm(&h,FROM_FREE_LIST);
            if(NULL != temp)
            {
               temp->data=i;
               put_node_in_list(&h,(node_sm_t *)temp,FROM_LIST|FROM_LIST);
            }
         }
#endif 
    }
    else if (strcmp(argv[1],"read")==0)
    {
        list_in_shm_init(&h,SHM_FILE,ID,sizeof(data_t),MAX_NODES,SEM_FILE,0);   
        for(i=0;i<MAX_NODES;i++)
        {
           temp=get_node_from_shm(&h,FROM_LIST);
           if(NULL != temp)
           {
              printf("\n %d %d",i,temp->data);
           }
        }
             
    }
    else if (strcmp(argv[1],"dump")== 0)
    {
       list_in_shm_init(&h,SHM_FILE,ID,sizeof(data_t),MAX_NODES,SEM_FILE,0);
       temp2=h.ptr;
       for(i=0;i<h.size;i++)
       {
             if(0 != i && i % 16 == 0 )
                printf("\n"); 
             printf("%02x",temp2[i]);    
       }  
    }
}
