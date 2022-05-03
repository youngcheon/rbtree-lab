#include "rbtree.h"
#include <stdlib.h>
#include <stdio.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  node_t *NIL = (node_t *)calloc(1, sizeof(node_t));
  NIL->color = RBTREE_BLACK;
  p->root=NIL;
  p->nil=NIL;
  return p;
}
void left_rotate(rbtree *t, node_t *p){
  // p의 오른쪽에 y노드 지정
  node_t *y = p->right;
  // y노드의 왼쪽노드를 p노드의 오른쪽으로 옮긴다.
  p->right = y->left;
  // y노드의 왼쪽노드에 노드가 존재한다면
  // 그 노드의 부모값을 x로 지정한다.
  if(y->left != t->nil){
    y->left->parent = p;
  }
  // y의 부모값을 p의 부모값으로 지정하여 원래 p가 있던 자리에 y를 넣어준다.
  y->parent = p->parent;
  // p가 root인 경우
  if (p->parent == t->nil){
    // 트리의 루트를 y로 지정해준다.
    t->root = y;
  // p가 왼쪽 자식인 경우
  }else if (p == p->parent->left){
    // p부모의 왼쪽노드를 y라고 해준다.
    p->parent->left = y;
  // p가 오른쪽 자식인 경우
  }else{
    //p부모의 오른쪽 노드를 y라고 해준다.
    p->parent->right = y;
  }
  //y의 왼쪽자식과 p의 부모노드 update
  y->left = p;
  p->parent = y;
}
void right_rotate(rbtree *t, node_t *p){
  node_t *y = p->left;
  p->left = y->right;
  if(y->right != t->nil){
    y->right->parent = p;
  }
  y->parent = p->parent;
  // p == root
  if (p->parent == t->nil){
    t->root = y;
  // p == right
  }else if (p == p->parent->right){
    p->parent->right = y;
  //p == left
  }else{
    p->parent->left = y;
  }
  y->right = p;
  p->parent = y;
}
void delete_node(rbtree *t, node_t *p){
  if(p != t->nil){
    delete_node(t, p->left);
    delete_node(t, p->right);
    free(p);
    p = NULL;
  }
}
void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  delete_node(t,t->root);
  free(t->nil);
  t->nil = NULL;
  free(t);
  t = NULL;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // 트리의 nil노드에 해당하는 y노드 생성
  node_t *y = t->nil;
  //트리의 루트에 해당하는 x노드 생성
  node_t *x = t->root;
  node_t *z = (node_t *)calloc(1, sizeof(node_t));
  z->key = key;
  //x 값이 nil 아닐때까지 탐색
  while(x != t->nil){
    // x가 nil을 만나기 전값을 y에 기록
    y = x;
    if(z->key < x->key){
      x = x->left;
    }else{
      x = x->right;
    }
  }
  z->parent = y;
  if (y==t->nil){
    t->root = z;
  }else if(z->key < y->key){
    y->left = z;
  }else{
    y->right = z;
  }
  z->left = t->nil;
  z->right = t->nil;
  z->color = RBTREE_RED;

  // fix up 
  while (z->parent->color == RBTREE_RED){
    // new node의 부모노드가 왼쪽 자식인 경우
    if(z->parent == z->parent->parent->left){
      // 부모 노드의 형제 노드
      y = z->parent->parent->right;
      // 경우 1 : 삼촌 색이 RED
      if (y->color == RBTREE_RED){
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      // 경우 2 && 3 : 삼촌 색이 BLACK
      }else{
        // 경우 2
        if(z == z->parent->right){
          z = z->parent;
          left_rotate(t,z);
        }
        //경우 3
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        right_rotate(t, z->parent->parent);
      }
    }else{ //new node의 부모노드가 오른쪽 자식일 경우
      // 삼촌 노드 정의
      y = z->parent->parent->left;
      // 경우 4 : 삼촌 RED
      if (y->color == RBTREE_RED){
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }else{
        // 경우 5 && 6
        if(z == z->parent->left){
          z = z->parent;
          right_rotate(t,z);
        }
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        left_rotate(t, z->parent->parent);
      }
    }
  }
  t->root->color = RBTREE_BLACK;
  return z;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  node_t *s = t->root;
  while(s && s->key != key){
    if(key < s->key){
      s = s->left;
    }else{
      s = s->right;
    }
  }
  return s;
}

node_t *rbtree_min(const rbtree *t) {
  node_t *n = t->root;
  while(n->left!=t->nil){
      n = n->left;
  }
  return n;
}

node_t *rbtree_max(const rbtree *t) {
  node_t *n = t->root;
  while(n->right!=t->nil){
      n = n->right;
  }
  return n;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase

  return 0;
}
int inorder(const rbtree *t, node_t *p, key_t *arr, const size_t n, int i){
  if (i < n){
    if (p == t->nil){
      return i;
    }
    i = inorder(t, p->left, arr, n, i);
    arr[i++] = p->key;
    i = inorder(t, p->right, arr, n, i);
    return i;
  }
  return 0;
}
int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  inorder(t, t->root, arr, n, 0);
  return 0;
}
