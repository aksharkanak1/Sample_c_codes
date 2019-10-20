#include "stdio.h"
#include "AVL.h"


int update_height_of_tree(avl_node_t * t )
{
    if ( NULL == t->l && NULL == t->r )
    {
        t->h=1;
    } 
    else if (NULL != t->l && NULL == t->r)
    {
       t->h=(t->l->h)+1;
    }
    else if (NULL == t->l && NULL != t->r)
    {
       t->h=(t->r->h)+1;
    }
    else if (NULL != t->l && NULL != t->r) 
    {
       t->h =1+((t->l->h > t->r->h)? t->l->h :t->r->h) ;  
    }
    return t->h; 
}

avl_node_t * leftRotate(avl_node_t * t)
{
    avl_node_t *t1 = NULL ;
    avl_node_t *t2 = NULL ;

    t1=t->r;
    t2=t1->l;

    t1->l=t;
    t->r=t2;

   
    update_height_of_tree(t);
    update_height_of_tree(t1);
   
    return t1;
}

avl_node_t* rightRotate(avl_node_t *t )
{
   avl_node_t * t1= NULL ;
   avl_node_t * t2 = NULL ;

   t1=t->l;
   t2=t1->r;

   t1->r=t;
   t->l=t2;

   update_height_of_tree(t);
   update_height_of_tree(t1);
   
   return t1;
   
}

int get_balance_factor(avl_node_t * t)
{
   return ((((t->l)!=NULL)?t->l->h:0) - (((t->r)!= NULL)?t->r->h:0));
}

void check_if_avl_tree_balanced(avl_node_t *t,int * is_balanced)
{
    int hl= 0;
    int hr=0;
    int bf=0;

    if(-1 == *is_balanced)
      return ;
   
    if ((NULL == t ) || (NULL == t->l && NULL == t->r )) 
       return ;

    check_if_avl_tree_balanced(t->l,is_balanced);
    check_if_avl_tree_balanced(t->r,is_balanced);

    bf = get_balance_factor(t);
 
    if(bf != 0 && bf != -1 && bf != 1)
      *is_balanced=-1;
    
    return ;
    
}

void inorder_treversal(avl_node_t * t)
{
    if (NULL == t)
       return ;

   inorder_treversal(t->l);
   printf("\n%d  height = %d t->l %x t->r %x",t->data,t->h,t->l,t->r);
   inorder_treversal(t->r);
}


avl_node_t* findMinChild(avl_node_t *t)
{
    avl_node_t*temp = t;
   
    while (NULL != temp->l)
    {
       temp=temp->l;
    }
    
    return temp ;
}


avl_node_t* insertAVLNode(avl_node_t *t , int k )
{
    avl_node_t * t1= NULL ;
    avl_node_t *t2= NULL ;
    int bf=0;
    if(NULL == t)
    {
        t1=(avl_node_t*) malloc (sizeof(avl_node_t));
        memset(t1,0,sizeof(avl_node_t));
        t1->data=k;
        t1->h=1;
        return t1; 
    }

    if (k < t->data )
    {
       t->l=insertAVLNode(t->l,k);
    }
    else if (k > t->data )
    {
       t->r=insertAVLNode(t->r,k);
    }
    else 
    {
        return t;    
    }

    update_height_of_tree(t) ;

    bf=get_balance_factor(t);
  
    t2=t;
 
    if(bf > 1 && k < t->l->data )
    {
       t2=rightRotate(t);
    }

    else if (bf > 1 && k > t->l->data )
    {
       t->l=leftRotate(t->l);
       t2=rightRotate(t);
    } 

    else if (bf < -1 && k >t->r->data )
    {
       t2=leftRotate(t);
    }

   else if (bf < -1 && k < t->r->data )
   {
        t->r=rightRotate(t->r);
        t2=leftRotate(t);
   }
  
   return  t2 ;
}


avl_node_t* deleteAVLNode(avl_node_t *t , int k )
{
     avl_node_t*  temp = NULL ;
     avl_node_t * t1= NULL ;
     avl_node_t *t2= NULL ;
     int bf=0;
  
     if (NULL == t)
        return NULL ; 

     if (k > t->data )
     {
         t->r=deleteAVLNode(t->r,k);
     }
     else if ( k < t->data )
     {
         t->l=deleteAVLNode(t->l,k);
     }
     else 
     {
          if (NULL == t->r && NULL == t->l )
          {
              free(t);
              return NULL ;
          }
          else if(NULL == t->r)
          { 
              temp=t->l;
              free(t);
              return  temp ;
          }
          else if(NULL == t->l)
          {
             temp=t->r;
             free(t);
             return temp ;
          }
          else 
          {
             temp=findMinChild(t);
             t->data=temp->data;
             t->l=deleteAVLNode(t->l,temp->data);
          }        
     }
 
     update_height_of_tree(t) ;

     bf=get_balance_factor(t);

     t2=t;

     if(bf > 1 && k < t->l->data )
     {
        t2=rightRotate(t);
     }

     else if (bf > 1 && k > t->l->data )
     {
        t->l=leftRotate(t->l);
        t2=rightRotate(t);
     }

     else if (bf < -1 && k >t->r->data )
     {
        t2=leftRotate(t);
     }

     else if (bf < -1 && k < t->r->data )
     {
        t->r=rightRotate(t->r);
        t2=leftRotate(t);
     }
 
     return t2;
       
}

avl_node_t * searchNode(avl_node_t * t , int data )
{
     avl_node_t * t1 = NULL ;
     
     t1 = t ;
   
     while(t1) 
     {
       if (data == t1->data )
          return t1 ;
       else if(data < t1->data )
       {
          t1=t1->l;
       }
       else if (data > t1->data)
       {
          t1=t1->r;
       }
     }

     return NULL ;
    
}





























