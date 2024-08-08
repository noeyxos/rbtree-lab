#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>

node_t SENTINEL = {
    .color = RBTREE_BLACK,
    .key = -1,
    .left = NULL,
    .right = NULL,
    .parent = NULL};

rbtree *new_rbtree(void)
{
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree)); // rbtree를 가리키는 포인터
  p->nil = &SENTINEL;
  p->root = p->nil;
  return p;
}

void free_node(rbtree *t, node_t *node)
{
  if (node != t->nil)
  {
    free_node(t, node->left);
    free_node(t, node->right);
    free(node);
  }
}

void delete_rbtree(rbtree *t)
{
  // TODO: reclaim the tree nodes's memory
  if (t != NULL)
  {
    free_node(t, t->root);
    free(t);
  }
}

void left_rotate(rbtree *t, node_t *parent)
{
  node_t *rotateNode = parent->right;
  parent->right = rotateNode->left;
  if (rotateNode->left != t->nil)
  {
    rotateNode->left->parent = parent;
  }
  rotateNode->parent = parent->parent;
  if (parent->parent == t->nil)
  {
    t->root = rotateNode;
  }
  else if (parent == parent->parent->left)
  {
    parent->parent->left = rotateNode;
  }
  else
  {
    parent->parent->right = rotateNode;
  }
  rotateNode->left = parent;
  parent->parent = rotateNode;
}

void right_rotate(rbtree *t, node_t *parent)
{
  node_t *rotate_node = parent->left;
  parent->left = rotate_node->right;
  if (rotate_node->right != t->nil)
  {
    rotate_node->right->parent = parent;
  }
  rotate_node->parent = parent->parent;
  if (parent->parent == t->nil)
  {
    t->root = rotate_node;
  }
  else if (parent == parent->parent->left)
  {
    parent->parent->left = rotate_node;
  }
  else
  {
    parent->parent->right = rotate_node;
  }
  rotate_node->right = parent;
  parent->parent = rotate_node;
}

node_t *rbtree_insert_fixup(rbtree *t, node_t *cur_node)
{
  while (cur_node->parent->color == RBTREE_RED)
  {
    if (cur_node->parent == cur_node->parent->parent->left)
    {
      node_t *uncle = cur_node->parent->parent->right;
      if (uncle->color == RBTREE_RED)
      {
        cur_node->parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        cur_node->parent->parent->color = RBTREE_RED;
        cur_node = cur_node->parent->parent;
      }
      else
      {
        if (cur_node == cur_node->parent->right)
        {
          cur_node = cur_node->parent;
          left_rotate(t, cur_node);
        }
        cur_node->parent->color = RBTREE_BLACK;
        cur_node->parent->parent->color = RBTREE_RED;
        right_rotate(t, cur_node->parent->parent);
      }
    }
    else
    {
      node_t *uncle = cur_node->parent->parent->left;
      if (uncle->color == RBTREE_RED)
      {
        cur_node->parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        cur_node->parent->parent->color = RBTREE_RED;
        cur_node = cur_node->parent->parent;
      }
      else
      {
        if (cur_node == cur_node->parent->left)
        {
          cur_node = cur_node->parent;
          right_rotate(t, cur_node);
        }
        cur_node->parent->color = RBTREE_BLACK;
        cur_node->parent->parent->color = RBTREE_RED;
        left_rotate(t, cur_node->parent->parent);
      }
    }
  }
  t->root->color = RBTREE_BLACK;
  return cur_node;
}

node_t *rbtree_insert(rbtree *t, const key_t key)
{
  // TODO: implement insert
  node_t *new_node = (node_t *)malloc(sizeof(node_t));
  new_node->color = RBTREE_RED;
  new_node->key = key;
  new_node->left = t->nil;
  new_node->right = t->nil;
  new_node->parent = t->nil;

  node_t *parent = t->nil;
  node_t *temp = t->root;
  while (temp != t->nil)
  {
    parent = temp;
    if (new_node->key < temp->key)
    {
      temp = temp->left;
    }
    else
    {
      temp = temp->right;
    }
  }
  new_node->parent = parent;

  if (parent == t->nil)
  {
    t->root = new_node;
  }
  else if (new_node->key < parent->key)
  {
    parent->left = new_node;
  }
  else
  {
    parent->right = new_node;
  }

  // new_node->left = t->nil;
  // new_node->right = t->nil;
  // new_node->color = RBTREE_RED;

  rbtree_insert_fixup(t, new_node);

  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key)
{
  // TODO: implement find
  node_t *temp = t->root;
  while (temp != t->nil)
  {
    if (key == temp->key)
    {
      return temp;
    }
    else if (key < temp->key)
    {
      temp = temp->left;
    }
    else
    {
      temp = temp->right;
    }
  }
  return NULL; // 키를 찾지 못한 경우 NIL 노드 반환
}

node_t *rbtree_min(const rbtree *t)
{
  // TODO: implement find
  node_t *temp = t->root;
  if (temp == t->nil)
  {
    return t->nil;
  }
  while (temp->left != t->nil)
  {
    temp = temp->left;
  }
  return temp;
}

node_t *rbtree_max(const rbtree *t)
{
  // TODO: implement find
  node_t *temp = t->root;
  if (temp == t->nil)
  {
    return t->nil;
  }
  while (temp->right != t->nil)
  {
    temp = temp->right;
  }
  return temp;
}

void rbtree_transplant(rbtree *t, node_t *u, node_t *v)
{
  if (u->parent == t->nil)
  {
    t->root = v;
  }
  else if (u == u->parent->left)
  {
    u->parent->left = v;
  }
  else
  {
    u->parent->right = v;
  }
  v->parent = u->parent;
}

void rbtree_erase_fixup(rbtree *t, node_t *x)
{
  while (x != t->root && x->color == RBTREE_BLACK)
  {
    if (x == x->parent->left)
    {
      node_t *w = x->parent->right;
      if (w->color == RBTREE_RED) // CASE 1
      {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        left_rotate(t, x->parent);
        w = x->parent->right;
      }
      if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK) // CASE 2
      {
        w->color = RBTREE_RED;
        x = x->parent;
      }
      else
      {
        if (w->right->color == RBTREE_BLACK) // CASE 3
        {
          w->left->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          right_rotate(t, w);
          w = x->parent->right;
        }
        w->color = x->parent->color; // CASE 4
        x->parent->color = RBTREE_BLACK;
        w->right->color = RBTREE_BLACK;
        left_rotate(t, x->parent);
        x = t->root;
      }
    }
    else
    { // 동일한 로직에서 방향만 반대인 경우
      node_t *w = x->parent->left;
      if (w->color == RBTREE_RED)
      {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        right_rotate(t, x->parent);
        w = x->parent->left;
      }
      if (w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK)
      {
        w->color = RBTREE_RED;
        x = x->parent;
      }
      else
      {
        if (w->left->color == RBTREE_BLACK)
        {
          w->right->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          left_rotate(t, w);
          w = x->parent->left;
        }
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->left->color = RBTREE_BLACK;
        right_rotate(t, x->parent);
        x = t->root;
      }
    }
  }
  x->color = RBTREE_BLACK;
}

node_t *rbtree_maximum(const rbtree *t, node_t *node)
{
  node_t *cur = node;
  while (cur->right != t->nil)
  {
    cur = cur->right;
  }
  return cur;
}

int rbtree_erase(rbtree *t, node_t *delete_node)
{
  // TODO: implement erase
  node_t *y = delete_node;
  node_t *delete_child_node;
  color_t y_original_color = y->color;
  if (delete_node->left == t->nil)
  {
    delete_child_node = delete_node->right;
    rbtree_transplant(t, delete_node, delete_node->right); // delete_node를 오른쪽 자식으로 바꾼다.
  }
  else if (delete_node->right == t->nil)
  {
    delete_child_node = delete_node->left;
    rbtree_transplant(t, delete_node, delete_node->left); // delete_node를 왼쪽 자식으로 바꾼다.
  }
  else
  {
    y = rbtree_maximum(t, delete_node->left); // y는 delete_node의 후손이다.
    y_original_color = y->color;
    delete_child_node = y->left;

    if (y != delete_node->left)
    {
      rbtree_transplant(t, y, y->left);
      y->left = delete_node->left;
      y->left->parent = y;
    }
    else
    {
      delete_child_node->parent = y;
    }
    rbtree_transplant(t, delete_node, y);
    y->right = delete_node->right;
    y->right->parent = y;
    y->color = delete_node->color;
  }
  if (y_original_color == RBTREE_BLACK)
  {
    rbtree_erase_fixup(t, delete_child_node);
  }
  return 0;
}

void _rec_set_array(key_t *arr, node_t *cur_node, int *cur_idx)
{
  if (cur_node->left == NULL)
    return;
  _rec_set_array(arr, cur_node->left, cur_idx);
  arr[*cur_idx] = cur_node->key;
  (*cur_idx) = (*cur_idx) + 1;
  _rec_set_array(arr, cur_node->right, cur_idx);
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n)
{
  // TODO: implement to_array
  node_t *cur_node = t->root;
  if (!cur_node)
    return 0;
  int start_idx = 0;
  // in order traversal with recursive function
  _rec_set_array(arr, cur_node, &start_idx);
  return 0;
}
