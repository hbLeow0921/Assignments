#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <unistd.h>
#include "bst.h"



struct _Node {
  int data;
  struct _Node* left;
  struct _Node* right;
};

Node* addNode(Node *root, int value)
{
  Node *newNode = malloc(sizeof(Node));
  newNode->data = value;
  newNode->left = NULL;
  newNode->right = NULL;

  Node *temp;
  Node *prev = NULL;

  temp = root;
  while(temp) {
    prev = temp;
    if (temp->data > newNode->data)
      temp = temp->right;
    else if (temp->data < newNode->data)
      temp = temp->left;
    else {
      free(newNode);
      return NULL;
    }
  }
  if (prev==NULL)
    root = newNode;
  else {
    if (prev->data > newNode->data)
      prev->right = newNode;
    else
      prev->left = newNode;
  }

  return root;
}

Node* freeSubtree(Node *N) {
  if (N == NULL)
    return NULL;
  freeSubtree(N->left);
  freeSubtree(N->right);
  free(N);
}

void removeSubtreeWorker(Node *R, int value) {
  if (R == NULL)
    return;

  if (R->left && R->left->data == value) {
    freeSubtree(R->left);
    R->left = NULL;
    return;
  }

  if (R->right && R->right->data == value) {
    freeSubtree(R->right);
    R->right = NULL;
    return;
  }

  if (R->data < value)
    removeSubtreeWorker(R->left, value);
  else
    removeSubtreeWorker(R->right, value);  
}

Node* removeSubtree(Node *R, int value) {
  if (R == NULL)
    return NULL;

  if (R->data == value) {
    freeSubtree(R);
    return NULL;
  }
  removeSubtreeWorker(R, value);
  return R;
}

void displaySubtree(Node *N)
{
  if (N==NULL)
    {
      return ;
    }
  displaySubtree(N->right);
  printf("%d\n", N->data);
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

// this is the most complicated task
Node* removeNode(Node* root, int value)
{
  // Ensure root isn't null.
  if (root == NULL) {
    return NULL;
  }
    
  if (value > root->data) {  // Value is in the left sub-tree.
    root->left = removeNode(root->left, value);
  } else if (value < root->data) { // Value is in the right sub-tree.
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
      root->data = temp->data; // duplicate the node
      root->left = removeNode(root->left, root->data); // delete the duplicate node
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

  int subDepth = nodeDepth(R->data < N->data ? R->left : R->right, N);

  if (subDepth >= 0)
    return subDepth + 1;
  else
    return -1;
}


//Average BST Function
float sumTree(Node* N){
  if(N == NULL)  //if empty tree return 0
    return 0;
  float sum;
  sum = (N->data + sumTree(N->left) + sumTree(N->right)); //traverse to get the sum of tree
  return sum;
}


float avgSubtree(Node *N)
{

  if(N == NULL)
    return -1;
  return sumTree(N)/countNodes(N); // return the average by having sum and total nodes inside tree

}


// This functions converts an unbalanced BST to a balanced BST

void addArray(Node * N, int arr[], int* i) 
{
  if (N == NULL) // return nothing if there is no binary tree 
    return ;

    
  
    
    addArray(N->right, arr, i); // use inorder function to store the bst into array 
    arr[*i] = N -> data;
    ++*i;
    addArray(N->left,arr,i);
 
}

Node* arrayToBst(int arr[],int first, int last)
{
    int mid; // use to find the middle of array to get the root
    Node* temp = malloc(sizeof(Node*)); 
    
    
    if(first > last) 
        return NULL;
    mid = first + (last - first)/2;
    
    temp = addNode(NULL,arr[mid]); //the middle of array will be root
    temp -> left = arrayToBst(arr,first,mid-1); // then for the left side will get the middle of the left hand side
    temp -> right = arrayToBst(arr,mid+1,last);// get the middle of the right hand side
    
    
    return temp; //return the node 
    
}




Node* balanceTree(Node* root)
{

    int i = 0;
    int* arr = malloc(sizeof(int)*countNodes(root)); 
    addArray(root,arr,&i); //add bst to sorted array
    root = arrayToBst(arr,0,countNodes(root)-1); // add sorted array to balanced bst
    
    return root; //return balanced bst

}