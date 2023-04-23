#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {

  TreeMap *arbol = (TreeMap*)malloc(sizeof(TreeMap));
  arbol->root = NULL;
  arbol->current = NULL;
  arbol->lower_than = lower_than;
  
  //new->lower_than = lower_than;
  return arbol;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {

  TreeNode *nodo = createTreeNode(key, value);

  if (tree->root == NULL) {
    tree->root = nodo;
    tree->current = nodo;
    return;
  }

  TreeNode* current = tree->root;
  TreeNode* parent = NULL;

    while(current != NULL) {
      
      if(is_equal(tree, key, current->pair->key) == 1) {
        return;
      }

      parent = current;
      if(tree->lower_than(key,current->pair->key) == 0) {
        current = current->right;
      } else current = current->left;
    }
      
    nodo->parent = parent;
    if (tree->lower_than(key, parent->pair->key) == 0) {
      parent->right = nodo;
    } else {
      parent->left = nodo;
    }
    
  tree->current = nodo;


  
}

TreeNode * minimum(TreeNode * x){

  if (x == NULL) return NULL;
  
  while(x->left != NULL) {
    x = x->left;
  }
  return x;
}


void removeNode(TreeMap * tree, TreeNode* node) {

  if(node == NULL) return;

  if (node->left == NULL && node->right == NULL)
  {
    if (node->parent == NULL) {
      tree->root = NULL;
    } 
    if(node->parent->right == node) {
      node->parent->right = NULL;
    } else if (node->parent->left == node) {
      node->parent->left = NULL;
    }
  }

  if((node->left != NULL && node->right == NULL) || (node->left == NULL && node->right != NULL)) {

    if(node->parent == NULL) {
      
      if(node->left != NULL) {
        tree->root = node->left;
      } else {
        tree->root = node->right;
      }
    }
    
    if(node->left != NULL) {
      if(node->parent->right == node) {
        node->parent->right = node->left;
        node->left->parent = node->parent;
        
      } else if (node->parent->left == node) {
        node->parent->left = node->left;
        node->left->parent = node->parent;
      }
      
    } else if (node->right != NULL) {
      if(node->parent->right == node) {
        node->parent->right = node->right;
        node->right->parent = node->parent;
      } else if (node->parent->left == node) {
        node->parent->left = node->right;
        node->right->parent = node->parent;
      }
    }
  }

  TreeNode* menorNodo = minimum(node->right);
  
  if(menorNodo != NULL) {
    node->pair->key = menorNodo->pair->key;
    node->pair->value = menorNodo->pair->value;
  }
 
  removeNode(tree, menorNodo);
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * arbol, void* key) {

  TreeNode *aux = arbol->root;
  
  while(aux != NULL){

    if(is_equal(arbol, key, aux->pair->key) == 1){

      arbol->current = aux;
      return aux->pair;
    }else if(arbol->lower_than(key,aux->pair->key) == 0){

      aux = aux->right;
      
    }else if(arbol->lower_than(key,aux->pair->key) == 1){

      aux = aux->left;
      
    }
  }
  
  
  return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {

  TreeNode* aux = tree->root;
  TreeNode* ub_node = NULL;
  while (aux != NULL) {
    if (is_equal(tree, key, aux->pair->key) == 1 ) {
      ub_node = aux;
      return ub_node->pair;
      
    } else if (tree->lower_than(key,aux->pair->key) == 0) {
      aux = aux->right;
    } else if (tree->lower_than(key,aux->pair->key) == 1) {
      ub_node = aux;
      aux = aux->left;
    }
  }
  if (ub_node != NULL) return ub_node->pair;
    
  return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {

  TreeNode* current = tree->root;
  current = minimum(current);
  tree->current = current;
  
  return current->pair;
}

Pair * nextTreeMap(TreeMap * tree) {

  if(tree->current->right != NULL) {
    tree->current = tree->current->right;
    tree->current = minimum(tree->current);
    return tree->current->pair;
  }

  TreeNode* parent = tree->current->parent;
  
  while(parent != NULL && tree->current == parent->right) {
    tree->current = parent;
    parent = parent->parent;
  }

  if(parent != NULL) {
    tree->current = parent;
    return tree->current->pair;
  }
    
  return NULL;
}
