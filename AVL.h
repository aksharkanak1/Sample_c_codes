#ifndef __AVL_H_
#define __AVL_H_


typedef struct avl_node_s {
        struct avl_node_s *l;
        struct avl_node_s *r;
        int data ;
        int h;
}avl_node_t;

avl_node_t* insertAVLNode(avl_node_t *t , int k );
void check_if_avl_tree_balanced(avl_node_t *t,int * is_balanced);
avl_node_t * searchNode(avl_node_t * t , int data );
#endif
