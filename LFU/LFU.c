#include "stdio.h"
#include <stdlib.h>
#include <string.h>
typedef struct node_s
{
   struct node_s *n;
   struct node_s *p;
}node_t ;

typedef struct Lst_s
{
   node_t * f;
   node_t * l ;
   int c ;
}Lst;
typedef struct fq_node_s
{
   node_t n;
   Lst lst ;
   int fq ;
}fq_node_t ;

typedef struct anode_s
{
    node_t h;
    node_t l;
    fq_node_t* fql;
    int key ;
    int data ;
    int fq;
}anode_t ;





#define HASH_BUCKET_SIZE  100

typedef struct LFUCache
{
  int c;
  int count;
  Lst *ht;
  Lst fqL;
} LFUCache;

node_t * remove_and_return_first_node(Lst*l);
void insert_at_start_list(Lst * l , node_t * node);
void insert_at_end_list(Lst *l , node_t *node );
void remove_node_from_lst(Lst * l , node_t * node);
int  put_new_node_in_fq_list(LFUCache*cache, anode_t * node )
{
    Lst * temp = NULL ;
    fq_node_t * tempfqNode= NULL ;
    if(NULL == cache || NULL == node )
       return ;

    node->fq=1;
    node->l.n=node->l.p=NULL ;
    temp=&(cache->fqL);
    if(0 == temp->c )
    {
        tempfqNode=(fq_node_t *)malloc(sizeof(fq_node_t ));
        if(NULL == tempfqNode )
        {
           return  -1 ;
        }
        memset(tempfqNode,0,sizeof(fq_node_t ));
        insert_at_start_list(temp,(node_t*)tempfqNode);
        tempfqNode->fq=1;
    }
    else
    {
        tempfqNode=(fq_node_t *)(cache->fqL.f);
    }
    insert_at_end_list(&(tempfqNode->lst),(node_t*)&(node->l));
    node->fql=tempfqNode;
    return 0;
}

void increase_fq_of_node(LFUCache*cache, anode_t * node )
{
    fq_node_t * fq_node= NULL ;
    fq_node_t * nfq_node=NULL ;
    node_t *temp= NULL ;
    int nfq=0;
    if(NULL == cache || NULL == node )
       return ;

    fq_node=node->fql;
    nfq=(node->fq)+1;

    nfq_node=(fq_node_t *)fq_node->n.n ;
    if(NULL == nfq_node )
    {
        nfq_node=(fq_node_t *) malloc(sizeof(fq_node_t ));
        if(NULL == nfq_node )
        {
            return  ;
        }
        memset(nfq_node,0,sizeof(fq_node_t ));
        insert_at_end_list(&(cache->fqL),(node_t*)&(nfq_node->n));
        nfq_node->fq=nfq;
    }
    remove_node_from_lst(&(node->fql->lst),(node_t*)&(node->l));
    insert_at_start_list(&(nfq_node->lst),(node_t*)&(node->l));
    node->fq=nfq;
    node->fql=nfq_node;
}

anode_t * remove_and_return_the_least_frequently_used_node(LFUCache*cache)
{
     node_t * temp ;
     Lst * tempfq= NULL ;
     if(NULL == cache || 0 == cache->fqL.c )
        return NULL ;
     tempfq=(&(((fq_node_t*)(cache->fqL.f))->lst));
     temp=remove_and_return_first_node(tempfq);
     if(NULL ==  temp)
        return NULL ;
     temp=temp-1;
     memset(&(((anode_t *)temp)->l), 0, sizeof(node_t));
     return ((anode_t *)temp);
}

anode_t * remove_anode_from_fq_list(anode_t *node)
{
     Lst *tempLst = NULL ;
     if(NULL == node || NULL == node->fql )
        return NULL ;

     tempLst=&(node->fql->lst);
     remove_node_from_lst(tempLst,(node_t*)&(node->l));
     return node ;
}

node_t * remove_and_return_first_node(Lst*l)
{
   node_t * ret= NULL ;
   if(NULL == l || 0 == l->c)
      return NULL  ;
   if(l->f == l-> l)
   {
       ret=l->f;
       l->f=l->l=NULL ;
       l->c=0;
   }
   else
   {
       ret=l->f;
       l->f=ret->n;
       l->f->p=NULL ;
      (l->c)--;
   }
   ret->n=ret->p=NULL ;
   return ret;
}

void insert_at_start_list(Lst * l , node_t * node)
{
   if(NULL == l || NULL == node )
     return  ;

   if(0 == l->c)
   {
       l->f=l->l=node;
       node->n=node->p=NULL ;
       (l->c)++;
   }
   else
   {
       node->n=l->f;
       node->p=NULL ;
       l->f->p=node;
       l->f=node;
       (l->c)++;
   }
}

void insert_at_end_list(Lst *l , node_t *node )
{
   if(NULL == l || NULL == node )
     return  ;
   if(0 == l->c)
   {
       l->f=l->l=node;
       node->n=node->p=NULL ;
       (l->c)++;
   }
   else
   {
       node->n=node->p=NULL ;
       node->p=l->l;
       l->l->n=node ;
       l->l=node;
       (l->c)++;
   }
}


void remove_node_from_lst(Lst * l , node_t * node)
{
   if(NULL == l || NULL == node)
   {
      return ;
   }

   if(NULL == node->p && NULL == node->n)
   {
       l->f=l->l=NULL ;
       l->c=0;
   }

   else if(NULL == node->p && NULL !=node->n )
   {
       l->f=node->n;
       l->f->p=NULL ;
       (l->c)--;
   }
   else if (NULL == node->n && NULL != node->p)
   {
       l->l=node->p;
       (l->c)--;
       l->l->n=NULL ;
   }
   else
   {
       node->p->n=node->n;
       node->n->p=node->p;
       (l->c)--;
   }
   node->n=node->p=NULL ;
}



anode_t *lookup_node(Lst *l,int key, int jump)
{
     node_t * n= NULL ;
     anode_t *temp=NULL ;
     char gotNode=0;

     if(0 == l->c)
       return NULL ;

     n=l->f;

     while(NULL != n )
     {
         temp=(anode_t*)(n-jump);
         if(key == temp->key)
         {
            gotNode=1;
            break;
         }
         n=n->n;
     }
     if(gotNode)
       return temp ;
     else
       return NULL ;
}

void remove_node_from_hash_table(LFUCache *obj,anode_t * node )
{
   int idx = 0 ;
   if(NULL == obj || NULL == node )
      return  ;

   idx=(node->key)% HASH_BUCKET_SIZE;
   remove_node_from_lst(&(obj->ht[idx]),(node_t*)(&(node->h)));
   memset(&(node->h),0,sizeof(node_t));
}



LFUCache* lFUCacheCreate(int capacity)
{
    LFUCache * temp = NULL ;

    temp =malloc(sizeof(LFUCache));
    if(NULL == temp )
       return NULL ;
    memset(temp,0,sizeof(LFUCache));
    temp->c=capacity;
    temp->ht=malloc(sizeof(Lst)*HASH_BUCKET_SIZE);
    if(NULL == temp->ht)
    {
       free(temp);
       return  NULL ;
    }
    memset(temp->ht, 0,(sizeof(Lst)*HASH_BUCKET_SIZE));
    return  temp ;
}

int lFUCacheGet(LFUCache* obj, int key)
{
    anode_t * temp= NULL ;
    int idx= 0 ;
    int ret=-1;

    idx=key%HASH_BUCKET_SIZE;
    temp=(anode_t*)lookup_node(&(obj->ht[idx]),key,0);
    if(NULL != temp )
    {
        increase_fq_of_node(obj,temp);
        ret=temp->data;
    }
    return ret;
}

void lFUCachePut(LFUCache* obj, int key, int value)
{
    anode_t * temp= NULL ;
    int idx= 0 ;

    idx=key%HASH_BUCKET_SIZE;

    temp=(anode_t*)lookup_node(&(obj->ht[idx]),key,0);
    if(NULL == temp )
    {
        if(obj->count < obj->c)
        {
            temp=malloc(sizeof(anode_t));
            if(NULL ==temp)
               return;
            memset(temp,0,sizeof(anode_t));
        }
        else
        {
            temp=remove_and_return_the_least_frequently_used_node(obj);
            remove_node_from_hash_table(obj,temp);
            memset(temp,0,sizeof(anode_t));
            (obj->count)--;
        }
        temp->data=value;
        temp->key=key ;
        put_new_node_in_fq_list(obj,temp);
        insert_at_end_list(&(obj->ht[idx]),(node_t*)temp);
        (obj->count)++;
   }
   else
   {
        temp->data=value;
        increase_fq_of_node(obj,temp);
   }
}

void lFUCacheFree(LFUCache* obj)
{
    anode_t * temp ;
    Lst *templst= NULL ;
    int i=0;

    for (i=0;i<HASH_BUCKET_SIZE;i++)
    {
        templst=&(obj->ht[i]);
        while(0 != templst->c )
        {
             temp=(anode_t *)remove_and_return_first_node(templst);
             if(NULL !=  temp)
             {
                 remove_anode_from_fq_list(temp);
                 free(temp);
             }
        }
    }

}

/**
 * Your LFUCache struct will be instantiated and called as such:
 * LFUCache* obj = lFUCacheCreate(capacity);
 * int param_1 = lFUCacheGet(obj, key);

 * lFUCachePut(obj, key, value);

 * lFUCacheFree(obj);
*/


int main()
{
   LFUCache *cache = NULL ;
   int ret=0;
   cache=lFUCacheCreate(2);
   lFUCachePut(cache,1,1);
   lFUCachePut(cache,2,2);
   ret=lFUCacheGet(cache,1);
   printf("\nret=%d",ret);
#if 0
   lFUCachePut(cache,3,3);
   ret=lFUCacheGet(cache,2);
   printf("\nret=%d",ret);
   ret=lFUCacheGet(cache,3);
   printf("\nret=%d",ret);
   lFUCachePut(cache,4,4);
   ret=lFUCacheGet(cache,1);
   printf("\nret=%d",ret);
   ret=lFUCacheGet(cache,3);
   printf("\nret=%d",ret);
   ret=lFUCacheGet(cache,4);
   printf("\nret=%d",ret);
#endif

   printf("\n");
}
