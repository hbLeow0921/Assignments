<html>
    <head>
        <title>Binary Search Tree in C</title>
        <h1>Implementing a binary search tree (BST) in C</h1>
        <p>
            Your task is to implement a BST for integers (int). In this exercise, we assume that there are no duplicate nodes. In bst.h, there is already a header file that declares the interface of that tree. You must implement the following functions for the tree operations in bst.c file. There is a file test_bst.c which is a simple testbench for testing your BST code. You may edit this file to include more test cases.
            Implement the following functions in the bst.c file.
        </p>
        
    </head>

    <body>
    <div>
        <h3>Task 1</h3>
        <h4>Node* addNode(Node *root, int value); </h4>
        <p> This function is used to insert a new node in the tree as a leaf node, and returns the leaf. The value of the new node is assigned the input ‘value’. During the insertion, a traversal starts from the root of the tree. The traversal follows the left branch if the value of the new node is greater than the value of the currently visited node. Otherwise the traversal follows the right branch. For this question we will restrict to the case that all data values are unique (that is to say, there will be no duplicates) for simplicity. In case of duplicate, the function returns NULL and does not insert a node. Also, this function is used to create a root node, when its argument root is NULL.</p>
           
        
        <h3>Task 2</h3>
        <h4>Node* removeNode(Node *root, int value);</h4>
        <p>This function is used to delete the node in the tree with the given value. For this question we will restrict to the case that all data values in the tree are unique. The function returns the new root, when is the root node to be deleted. Nothing happens if the value is not present.</p>

        <h3>Task 3</h3>
        <h4>void displaySubtree(Node *N);</h4>
        <p>This function is for in-order printing of the node values. It prints
            i) the values of the right subtree ii) the value of the node
            iii) finally, the values of the left subtree
            Thus, the function prints the node values in sorted ascending format.
            The function should print one node value per line, for example: </p>
            <p>5</p>
            <p>4</p>
            <p>3</p>
            <p>2</p>
            <p>Not 5 4 3 2.</p>

        <h3>Task 4</h3>
        <h4>int leavesNumber(Node *N);</h4>
        <p>This function returns the number of leaves in the subtree rooted at node N.</p> 

        <h3>Task 5</h3>
        <h4>Node* removeSubtree(Node *root, int value);</h4>
        <p>This function deletes the entire subtree rooted at the node with the given value. The function returns the new root, like delete node. If the value is not present, then nothing should happen.</p>

        <h3>Task 6</h3>
        <h4>int nodeDepth (Node * root, Node *N);</h4>
        <p>This function returns the depth of node N from root R. The depth is the exact number of edges between R and N. If N=R the depth is 0. If N does not belong to the tree rooted in R, then this function returns NULL.</p>
    </div>
    </body>
</html>
