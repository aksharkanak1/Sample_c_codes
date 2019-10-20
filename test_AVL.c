#include "stdio.h"
#include <stdlib.h>
#include "AVL.h"
int main(int argc , char * argv[])
{
    unsigned int num=0;
    unsigned int i =0;
    unsigned int temp= NULL ;
    avl_node_t * tree= NULL ;
    avl_node_t *tempTree=NULL ;
    int temp2=0;

    if (argc != 2)
       return -1 ;  

    num=atoi(argv[1]);
    for (i =1; i<num+1 ;i++)
    {
       //gen_random_num(&temp,0);
       tree=insertAVLNode(tree,i);
    }
   
    gen_random_num(&temp,1);


    printf("\nPrinting the inorder treversal of the tree\n");
    inorder_treversal(tree);
    printf("\n");

#if 0
    check_if_avl_tree_balanced(tree,&temp2);

    printf("\n Is balanced is %d" , temp2);

    printf("\n Search for the char");
    scanf("%d",&temp2);
    tempTree=searchNode(tree,temp2);
  
    if (NULL == tempTree )
       printf("\n Node not found ");
    else 
       printf("\n Node found "); 
#endif 
    printf("\nThe root node data is %d height %d\n",tree->data,tree->h);  
    printf("\n");
  
    printf("\nGet the node to be deleted ");
    scanf("%d",&num);
   
    tree=deleteAVLNode(tree,num);

    check_if_avl_tree_balanced(tree,&temp2);

    printf("\nPrinting the inorder treversal of the tree\n");
    inorder_treversal(tree);
    printf("\n");
    
    printf("\n Is balanced is %d" , temp2);

    
 
}
