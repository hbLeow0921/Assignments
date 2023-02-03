#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <unistd.h>
#include "bst.h"

struct _Node {
  int value;
  struct _Node* left;
  struct _Node* right;
};

Node* addNode(Node *root, int value)
{
  Node *newNode = malloc(sizeof(Node));

  newNode->value = value;
  newNode->left = NULL;
  newNode->right = NULL;

  Node *temp;
  Node *prev = NULL;
  temp = root;

  while(temp) {
    prev = temp;
    //Traverse the tree to find the correct location for the new node
    if (temp->value > newNode->value) 
      temp = temp->right;
    else if (temp->value < newNode->value)
      temp = temp->left;
    else {
      //Free Duplicate Node
      free(newNode);
      return NULL;
    }
  }
  if (prev==NULL)
    root = newNode;
  else {//Add the new node to the tree
    if (prev->value > newNode->value)
      prev->right = newNode;
    else
      prev->left = newNode;
  }
  return newNode; // return the new node rather than the root
}

//Free the memory allocated to the tree
void freeSubtree(Node *N) {
  if (N == NULL)
    return;
  freeSubtree(N->left);
  freeSubtree(N->right);
  free(N);
}


void removeSubtreeWorker(Node *R, int value) {
  if (R == NULL)
    return;
  if (R->left && R->left->value == value) {
    freeSubtree(R->left);
    R->left = NULL;
    return;
  }
  if (R->right && R->right->value == value) {
    freeSubtree(R->right);
    R->right = NULL;
    return;
  }
  if (R->value < value)
    removeSubtreeWorker(R->left, value);
  else
    removeSubtreeWorker(R->right, value);  
}

Node* removeSubtree(Node *R, int value) {
  if (R == NULL)
    return NULL;
  if (R->value == value) {
    freeSubtree(R);
    return NULL;
  }
  removeSubtreeWorker(R, value);
  return R;
}

//Display the Tree using inorder traversal
void displaySubtree(Node *N)
{
  if (N==NULL)
    {
      return ;
    }
  displaySubtree(N->right);
  printf("%d\n", N->value);
  displaySubtree(N->left);
}

int countNodes(Node *N)
{
  int count=0;
  if(N==NULL)
    return 0;
  count = count + countNodes(N->right);
  count = count + countNodes(N->left);
  count = count + 1;
  return count;
}

Node* removeNode(Node* root, int value)
{
  // Ensure root isn't null.
  if (root == NULL) {
    return NULL;
  }
  
  if (value > root->value) {  // Value is in the left sub-tree.
    root->left = removeNode(root->left, value);
  } else if (value < root->value) { // Value is in the right sub-tree.
    root->right = removeNode(root->right, value);
  } else { // Found the correct node with value
    // Check the three cases - no child, 1 child, 2 child...
    // No Children
    if (root->left == NULL && root->right == NULL) {
      free(root);
      root = NULL;
    }
    // 1 child (on the right)
    else if (root->left == NULL) {
      Node *temp = root; // save current node
      root = root->right;
      free(temp);
    }
    // 1 child (on the left)
    else if (root->right == NULL) {
      Node *temp = root; // save current node
      root = root->left;
      free(temp);
    }
    // Two children
    else {
      // find minimal value of right sub tree
      Node *temp = root->left; 
      while(temp->right != NULL) {
        temp = temp->right;
      }
      root->value = temp->value; // duplicate the node
      root->left = removeNode(root->left, root->value); // delete the duplicate 
node
    }
  }
  return root; // parent node can update reference
}

int numberLeaves(Node *N) {
  if (N == NULL)
    return 0;
  if (N->left == NULL && N->right == NULL)
    return 1;
  return numberLeaves(N->left) + numberLeaves(N->right);
}

int nodeDepth(Node *R, Node *N) {
  if (R == NULL || N == NULL)
    return -1;
  if (R == N)
    return 0;
  int subDepth = nodeDepth(R->value < N->value ? R->left : R->right, N);
  if (subDepth >= 0)
    return subDepth + 1;
  else
    return -1;
}