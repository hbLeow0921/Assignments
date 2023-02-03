#include<string.h>
pthread_rwlock_t try_lock = PTHREAD_RWLOCK_INITIALIZER; //using read write lock to intialize a lock that provide 2 functions to maximize the performance

void *downtime() {  
    
    Node* balTree = NULL;//create a node pointer to balance the tree
    for(int i = 0; i < 3; i++){ //use to end function after downtime was called for 3 times
        sleep(2); //call downtime function every 2 seconds
        pthread_rwlock_wrlock(&try_lock); //using write lock to balance the tree
        balTree = balanceTree(root); //use char pointer to balance the tree
        root = freeSubtree(root); //old tree will be free
        root = balTree; //new balanced tree will be provided
      pthread_rwlock_unlock(&try_lock); //unlock after operation
    }
    return NULL;

}

void *ServeClient(char *client)
{
	int value;
	char str[20];
	FILE* fp = fopen(client , "r");
	if(fp == NULL)
	{
	return NULL;
	}
	else{
		char clientName[30];
		strcpy(clientName, client);
		strtok(clientName, "_");/
		int ret = 0;
		while(ret > -1)//Goes through the whole file until the end of the file
		{
			ret = fscanf(fp, "%s %d" , str,  &value); //Scans the line in the file and it will either return 1,2 or 0 depending on the number of items it has successfully matched
			if(ret == 2)//If it has 2 items (Function and value)
			{
				if(str[0] == 'a')//If the String starts with a, the client wants to add node
				{
					pthread_rwlock_wrlock(&lock);//Lock using wrlock because it is a write and read operation
					root = addNode(root, value);// Implement the addNode with the value the client inputted
					printf("%sinsertNode=%d\n" , clientName, value);//Print in the log that it has successfully inserted node
					pthread_rwlock_unlock(&lock);//Unlock the wrlock after everything is done
					
				}
				
				else if(str[0] == 'r') //If the String starts with r, the client wants to remove node
				{
					pthread_rwlock_wrlock(&lock);//Lock using wrlock because it is a write and read operation
					root = removeNode(root , value);// Implement the remoeNode with the value the client inputted
					printf("%sdeleteNode=%d\n" , clientName, value);//Print in the log that it has successfully inserted node
					pthread_rwlock_unlock(&lock);//Unlock the wrlock after everything is done
		
				}
			}
			else if(ret == 1)//If it has 1 item (Function)
			{
				if(str[0] == 'c')//If the String starts with c, the client wants to count node
				{
					pthread_rwlock_rdlock(&lock);//Lock using rdlock because it is a read operation
					printf("%scountNode=%d\n" , clientName, countNodes(root));//Implements the countNodes function while also returning the value that it gives to the log
					pthread_rwlock_unlock(&lock);//unlock the rdlock adter everything is done
					
				}
				
				else if(str[0] == 'a')//If the String starts with a, the client wants to get the average of nodes
				{
					pthread_rwlock_rdlock(&lock);//Lock using rdlock because it is a read operation
					printf("%savgSubtree=%f\n" , clientName, avgSubtree(root));//Implements the avgSubtree function while also returning the value that it gives to the log
					pthread_rwlock_unlock(&lock);//unlock the rdlock adter everything is done
				}
			}
			
			
		}
	}
	fclose(fp);
	return NULL;
}
