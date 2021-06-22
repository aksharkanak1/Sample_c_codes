#include "list_in_shm.h"


int list_in_shm_init(list_in_shm_handle_t * h, char * file , uint32_t id ,uint32_t size,uint32_t num,char *nameSem,uint8_t init )
{
    uint64_t totalSize=0;
    uint32_t i =0; 
    uint8_t *temp = NULL ;
    if (NULL == h || NULL == file || NULL == nameSem )
       return  -1;

    h->key=ftok(file,id);

    if(-1 == h->key )
    {
       printf("\nftok failed for %s %x  with errono %u\n",file,id,errno);
       return -1;
    }

    h->sem=sem_open(nameSem,O_CREAT,O_WRONLY,1);
    if (NULL  == h->sem )
    {
       printf("\nsem_open failed for file %s  %u\n",nameSem,errno);
       return -1 ;
    }

    totalSize=(size * num)+sizeof(sm_list_t)+sizeof(sm_list_t);

    h->shmid = shmget(h->key,totalSize,IPC_CREAT);
    if ( -1 == h->shmid )
    {
       printf("\nshmget failed errono %u\n",errno);
       sem_close(h->sem);
       return -1;
    }
    h->size=totalSize;
    h->ptr=shmat(h->shmid,NULL,0);
    if((void *) -1 == h->ptr)
    {
       printf("\nshmat failed  %u\n",errno);
       sem_close(h->sem);
       return  -1;
    }

    h->pFreeListStruct=(sm_list_t*)(h->ptr);
    h->pList=(sm_list_t*)((uint8_t*)(h->pFreeListStruct)+sizeof(sm_list_t));
    h->pStart=(uint8_t*)((uint8_t*)(h->pList)+sizeof(sm_list_t));

    if(init)
    {
         sem_wait(h->sem);
         memset(h->ptr,0,totalSize);
         temp=h->pStart;
         for(i=0;i<num;i++)
         {
            list_in_shm_insert_node(h->pFreeListStruct,h->ptr,(node_sm_t *)(temp+i*size),FROM_FRONT);
         }
         sem_post(h->sem);
    }
    return 0 ;
} 


int list_in_shm_insert_node(sm_list_t * plst, uint8_t * basePtr , node_sm_t * node ,uint8_t front)
{
    node_sm_t* temp = NULL ;

    if(NULL == plst || NULL == basePtr || NULL == node )
      return -1 ;

    if ( 0 == plst->p_off  && 0 == plst->n_off)
    {
         plst->p_off=GET_OFFSET(basePtr,node);
         plst->n_off=GET_OFFSET(basePtr,node);
         plst->c=1;
         return 0;
    } 
 
    if(FROM_FRONT & front)
    {
       temp=(node_sm_t*)(GET_PTR(basePtr,plst->n_off));
       node->n_off=plst->n_off;
       temp->p_off=GET_OFFSET(basePtr,node);
       node->p_off =0;
       plst->n_off=GET_OFFSET(basePtr,node);
       (plst->c)++;
    }
    else 
    {
        temp=(node_sm_t*)(GET_PTR(basePtr,plst->p_off));
        temp->n_off=GET_OFFSET(basePtr,node);
        node->p_off=plst->p_off;
        node->n_off=0;
        plst->p_off=GET_OFFSET(basePtr,node);
       (plst->c)++;    
    }
    return 0;   
}

node_sm_t *list_in_shm_get_node(sm_list_t * plst,uint8_t * basePtr,uint8_t front)
{
   node_sm_t * temp = NULL ;
   node_sm_t * temp2 = NULL ;
   uint32_t offtemp = 0; 
   if(NULL == plst || NULL == basePtr || (0 == plst->c))
     return NULL ;

   if(1 == plst->c )
   {
       temp=(node_sm_t *)(GET_PTR(basePtr,plst->n_off));
       plst->c=0;
       plst->n_off=0;
       plst->p_off=0;
       return temp ;
   } 
   if(FROM_FRONT & front)
   {
      temp=(node_sm_t *)(GET_PTR(basePtr,plst->n_off));
      offtemp=temp->n_off;
      plst->n_off=offtemp; 
      temp2=(node_sm_t *)(GET_PTR(basePtr,plst->n_off));
      temp2->p_off=0;
      (plst->c)--; 
   }
   else
   {
      temp=(node_sm_t *)(GET_PTR(basePtr,plst->p_off));      
      offtemp=temp->p_off;
      plst->p_off=offtemp;
      temp2=(node_sm_t *)(GET_PTR(basePtr,offtemp));
      temp2->n_off=0; 
      (plst->c)--;
   }

   return temp ;
}

node_sm_t * get_node_from_shm(list_in_shm_handle_t * h,uint8_t flags )
{
    sm_list_t *lst=NULL ;
    node_sm_t * temp = NULL ;
    int ret=0;
    if (NULL == h )
    {
        return NULL ;
    }
 
    ret=sem_wait(h->sem);
    if(0 != ret ) 
    {
       printf("\n(%s) failed to get sem",__FUNCTION__);
       return  NULL ;
    }

    if(FROM_FREE_LIST & flags )
    {
       lst=h->pFreeListStruct;
    }
    else 
    {
      lst=h->pList;
    }
    if (0 == lst || 0 == lst->c)
    {
       sem_post(h->sem);
       return NULL ;
    }
    temp=list_in_shm_get_node(lst,h->ptr,flags);
    sem_post(h->sem);
    return temp ; 
}

int put_node_in_list(list_in_shm_handle_t * h,node_sm_t *node,uint8_t flags)
{
    sm_list_t *lst=NULL ;
    int ret=0;
    if (NULL == h || NULL == node)
    {
        return -1 ;
    }

    ret=sem_wait(h->sem);
    if(0 != ret )
    {
       printf("\n(%s) failed to get sem",__FUNCTION__);
       return  -1 ;
    }

    if(FROM_FREE_LIST & flags )
    {
       lst=h->pFreeListStruct;
    }
    else
    {
      lst=h->pList;
    }
 
    ret=list_in_shm_insert_node(lst, h->ptr, node ,flags);  
    sem_post(h->sem);
    return 0;   
}

void list_in_shm_finish(list_in_shm_handle_t * h)
{
     sem_close(h->sem);
     shmdt(h->ptr);
}
