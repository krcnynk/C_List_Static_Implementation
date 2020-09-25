#include "list.h"
#include <stddef.h>
#include <stdio.h>

static Node nodes[LIST_MAX_NUM_NODES_OOB];
static List lists[LIST_MAX_NUM_HEADS];
static unsigned int removedNodes[LIST_MAX_NUM_NODES_OOB] = {[0] =(LIST_MAX_NUM_NODES_OOB - 1)};
static unsigned int removedIndex = 0;
static bool listAvailable[LIST_MAX_NUM_HEADS];

//Changes

// Makes a new, empty list, and returns its reference on success. 
// Returns a NULL pointer on failure.
List* List_create()
{
	for(int i = 0;i < LIST_MAX_NUM_HEADS;++i)
	{
		if(!lists[i].inUse)
		{
			lists[i].inUse = true;
			lists[i].head = LIST_OOB_START;
			lists[i].head = LIST_OOB_END;
			lists[i].currNode = LIST_OOB_START;
			lists[i].itemCount = 0;
			return &(lists[i]);
		}
	}

	return NULL;
}

// Returns the number of items in pList.
int List_count(List* pList)
{
	return pList->itemCount;
}

// Returns a pointer to the first item in pList and makes the first item the current item.
// Returns NULL and sets current item to NULL if list is empty.
void* List_first(List* pList)
{
	if(pList->itemCount != 0)
	{
		pList->currNode = pList->head;
		return nodes[pList->currNode].itemP;
	}
	else
	{
		nodes[pList->currNode].itemP = NULL;
		return NULL;
	}
}

// Returns a pointer to the last item in pList and makes the last item the current item.
// Returns NULL and sets current item to NULL if list is empty.
void* List_last(List* pList)
{
	if(pList->itemCount != 0)
	{
		pList->currNode = pList->tail;
		return nodes[pList->currNode].itemP;
	}
	else
	{
		nodes[pList->currNode].itemP = NULL;
		return NULL;
	}
}

// Advances pList's current item by one, and returns a pointer to the new current item.
// If this operation advances the current item beyond the end of the pList, a NULL pointer 
// is returned and the current item is set to be beyond end of pList.
void* List_next(List* pList)
{
	if(pList->itemCount == 0)
	{
		return NULL;
	}

	if(pList->currNode == LIST_OOB_END)
	{
		return NULL;
	}
	else if(nodes[pList->currNode].nextNode == LIST_OOB_END)
	{
		pList->currNode = LIST_OOB_END;
		return NULL;
	}
	else
	{
		pList->currNode = nodes[pList->currNode].nextNode;
		return nodes[pList->currNode].itemP;
	}

}

// Backs up pList's current item by one, and returns a pointer to the new current item. 
// If this operation backs up the current item beyond the start of the pList, a NULL pointer 
// is returned and the current item is set to be before the start of pList.
void* List_prev(List* pList)
{
	if(pList->itemCount == 0)
	{
		return NULL;
	}

	if(pList->currNode == LIST_OOB_START)
	{
		return NULL;
	}
	else if(nodes[pList->currNode].prevNode == LIST_OOB_START)
	{
		pList->currNode = LIST_OOB_START;
		return NULL;
	}
	else
	{
		pList->currNode = nodes[pList->currNode].prevNode;
		return nodes[pList->currNode].itemP;
	}
}

// Returns a pointer to the current item in pList.
void* List_curr(List* pList)
{
	return nodes[pList->currNode].itemP;
}

static int getFreeNodeHandler()
{
	//Handles removal backtracking(removedIndex 0 means non disjoint array, 0> disjoint)
	int NodeIndex;

	//Node full, NO SPACE
	if(removedIndex == 0 && removedNodes[removedIndex] == 1)
	{
		return -1;
	}
	//Not disjoint but there is some space
	else if(removedIndex == 0)
	{
		NodeIndex = removedNodes[removedIndex]--;
	}
	//Disjoint and there is some space, fill the gap
	else
	{
		NodeIndex = removedNodes[removedIndex--];
	}

	return NodeIndex;
}

// Adds the new item to pList directly after the current item, and makes item the current item. 
// If the current pointer is before the start of the pList, the item is added at the start. If 
// the current pointer is beyond the end of the pList, the item is added at the end. 
// Returns 0 on success, -1 on failure.
int List_add(List* pList, void* pItem)
{
	int NodeIndex = getFreeNodeHandler();
	if(NodeIndex < 0)
	{
		return -1;
	}

	// If the list is empty
	if(pList->itemCount == 0)
	{
		pList->head = NodeIndex;
		pList->tail = NodeIndex;
		pList->currNode = NodeIndex;

		nodes[pList->currNode].itemP = pItem;
		nodes[pList->currNode].nextNode = LIST_OOB_END;
		nodes[pList->currNode].prevNode = LIST_OOB_START;
		
	}
	else
	{
		//Current pointer before start
		if(pList->currNode == LIST_OOB_START)
		{
			nodes[pList->head].prevNode = NodeIndex;
			nodes[NodeIndex].nextNode = pList->head;
			nodes[NodeIndex].prevNode = LIST_OOB_START;
			nodes[NodeIndex].itemP = pItem;
			pList->head = NodeIndex;
			pList->currNode = NodeIndex;
		}
		//Current pointer after end
		else if(pList->currNode == LIST_OOB_END)
		{
			nodes[pList->tail].nextNode = NodeIndex;
			nodes[NodeIndex].prevNode = pList->tail;
			nodes[NodeIndex].nextNode = LIST_OOB_END;
			nodes[NodeIndex].itemP = pItem;
			pList->tail = NodeIndex;
			pList->currNode = NodeIndex;
		}
		//Current pointer at the last node(one item or last node)
		else if(pList->currNode == pList->tail)
		{
			nodes[pList->currNode].nextNode = NodeIndex;
			nodes[NodeIndex].prevNode = pList->currNode;
			nodes[NodeIndex].nextNode = LIST_OOB_END;
			nodes[NodeIndex].itemP = pItem;
			pList->currNode = NodeIndex;
			pList->tail = NodeIndex;
		}
		// Current pointer has a front neighbour
		else
		{
			unsigned int nextNodeTemp = nodes[pList->currNode].nextNode;
			nodes[pList->currNode].nextNode = NodeIndex;
			nodes[NodeIndex].prevNode = pList->currNode;
			nodes[NodeIndex].nextNode = nextNodeTemp;
			nodes[NodeIndex].itemP = pItem;
			nodes[nextNodeTemp].prevNode = NodeIndex;
			pList->currNode = NodeIndex;
		}
	}
	++(pList->itemCount);
	return 0;
}

// Adds item to pList directly before the current item, and makes the new item the current one. 
// If the current pointer is before the start of the pList, the item is added at the start. 
// If the current pointer is beyond the end of the pList, the item is added at the end. 
// Returns 0 on success, -1 on failure.
int List_insert(List* pList, void* pItem)
{
	if(pList->currNode == LIST_OOB_START || pList->currNode == LIST_OOB_END)
	{
		return List_add(pList,pItem);
	}
	else
	{
		pList->currNode = nodes[pList->currNode].prevNode;
		return List_add(pList,pItem);
	}
	
}

// Adds item to the end of pList, and makes the new item the current one. 
// Returns 0 on success, -1 on failure.
int List_append(List* pList, void* pItem)
{
	pList->currNode = LIST_OOB_END;
	return List_add(pList,pItem);
}

// Adds item to the front of pList, and makes the new item the current one. 
// Returns 0 on success, -1 on failure.
int List_prepend(List* pList, void* pItem)
{
	pList->currNode = LIST_OOB_START;
	return List_add(pList,pItem);
}

static void removeNodeHandler(int NodeIndex)
{

	//Handles removal backtracking
	removedNodes[++removedIndex] = NodeIndex;
}
// Return current item and take it out of pList. Make the next item the current one.
// If the current pointer is before the start of the pList, or beyond the end of the pList,
// then do not change the pList and return NULL.
void* List_remove(List* pList)
{
	void* item;

	if(pList->currNode == LIST_OOB_END || pList->currNode == LIST_OOB_START || pList->itemCount == 0)
	{
		return NULL;
	}
	else
	{
		item = nodes[pList->currNode].itemP;
		removeNodeHandler(pList->currNode);

		//If there is only one item in the list(head == tail)
		if(pList->head == pList->tail)
		{
			//Reset the node
			nodes[pList->currNode].itemP = NULL;
			nodes[pList->currNode].nextNode = 0; //UNNECESSARY
			nodes[pList->currNode].prevNode = 0;//UNNECESSARY
			//Reset the list 
			pList->head = 0;
			pList->tail = 0;
			pList->currNode = 0;
			pList->itemCount = 0;
		}
		//If current node is looking at head and multiple entries
		else if(pList->currNode == pList->head)
		{

			//Save temp data
			unsigned int tempNextNode = nodes[pList->currNode].nextNode;
			item = nodes[pList->currNode].itemP;
			//Reset the node
			nodes[pList->currNode].itemP = NULL;
			nodes[pList->currNode].nextNode = 0;
			nodes[pList->currNode].prevNode = 0;//UNNECESSARY
			//Fix next nodes prev connection
			nodes[tempNextNode].prevNode = LIST_OOB_START;
			//Change head and current to next
			pList->head = tempNextNode;
			pList->currNode = tempNextNode;
			--pList->itemCount;
		}
		//If current node is looking at tail and multiple entries
		else if(pList->currNode == pList->tail)
		{
			//Save temp data
			unsigned int tempPrevNode = nodes[pList->currNode].prevNode;
			item = nodes[pList->currNode].itemP;
			//Reset the node
			nodes[pList->currNode].itemP = NULL;
			nodes[pList->currNode].nextNode = 0;//UNNECESSARY
			nodes[pList->currNode].prevNode = 0;
			//Fix prev nodes next connection
			nodes[tempPrevNode].nextNode = LIST_OOB_END;
			//Change tail and current to next
			pList->tail = tempPrevNode;
			pList->currNode = tempPrevNode;
			--pList->itemCount;
		}
		//Somewhere in middle
		else
		{
			//Save temp data
			item = nodes[pList->currNode].itemP;
			unsigned int tempPrevNode = nodes[pList->currNode].prevNode;
			unsigned int tempNextNode = nodes[pList->currNode].nextNode;
			//Reset the node
			nodes[pList->currNode].itemP = NULL;
			nodes[pList->currNode].nextNode = 0;//UNNECESSARY
			nodes[pList->currNode].prevNode = 0;
			//Fix nodes connection
			nodes[tempPrevNode].nextNode = tempNextNode;
			nodes[tempNextNode].prevNode = tempPrevNode;
			//Change current to next
			pList->currNode = tempNextNode;
			--pList->itemCount;
		}
		return item;
	}
}

// Adds pList2 to the end of pList1. The current pointer is set to the current pointer of pList1. 
// pList2 no longer exists after the operation; its head is available
// for future operations.
void List_concat(List* pList1, List* pList2)
{
	if(pList1->itemCount == 0 || pList2->itemCount == 0)
	{
		return;
	}
	else
	{
		pList1->itemCount += pList2->itemCount;
		nodes[pList1->tail].nextNode = pList2->head;
		pList1->tail = pList2->tail;
		pList2->inUse = false;
	}
}
// Delete pList. pItemFreeFn is a pointer to a routine that frees an item. 
// It should be invoked (within List_free) as: (*pItemFreeFn)(itemToBeFreedFromNode);
// pList and all its nodes no longer exists after the operation; its head and nodes are 
// available for future operations.
typedef void (*FREE_FN)(void* pItem);
void List_free(List* pList, FREE_FN pItemFreeFn)
{
	pList->currNode = pList->head;
	void* itemPtr = List_remove(pList);
	while(itemPtr != NULL)
	{
		(*pItemFreeFn)(itemPtr);
	}
	pList->inUse = false;
}
// Return last item and take it out of pList. Make the new last item the current one.
// Return NULL if pList is initially empty.
void* List_trim(List* pList)
{
	if(pList->itemCount == 0)
	{
		return NULL;
	}
	pList->currNode = pList->tail;
	int beforeTail = nodes[pList->tail].prevNode;
	void* itemPTR = List_remove(pList);
	pList->currNode = beforeTail;
	return itemPTR;
}
// Search pList, starting at the current item, until the end is reached or a match is found. 
// In this context, a match is determined by the comparator parameter. This parameter is a
// pointer to a routine that takes as its first argument an item pointer, and as its second 
// argument pComparisonArg. Comparator returns 0 if the item and comparisonArg don't match, 
// or 1 if they do. Exactly what constitutes a match is up to the implementor of comparator. 
// 
// If a match is found, the current pointer is left at the matched item and the pointer to 
// that item is returned. If no match is found, the current pointer is left beyond the end of 
// the list and a NULL pointer is returned.
// 
// If the current pointer is before the start of the pList, then start searching from
// the first node in the list (if any).
typedef bool (*COMPARATOR_FN)(void* pItem, void* pComparisonArg);
void* List_search(List* pList, COMPARATOR_FN pComparator, void* pComparisonArg)
{
	
}





void printAllNodes()
{
	for(int i = 0;i<LIST_MAX_NUM_NODES_OOB;++i)
	{
		if(nodes[i].itemP != NULL)
			printf("Item : %d, Next : %u, Prev : %u --> \n",*(unsigned int*)nodes[i].itemP,nodes[i].nextNode,nodes[i].prevNode);
		else
			printf("Item : %d, Next : %u, Prev : %u --> \n",-1,nodes[i].nextNode,nodes[i].prevNode);
	}
}
void printAllLists()
{
	for(int i = 0;i<LIST_MAX_NUM_HEADS;++i)
	{
		printf("head : %u, tail : %u, currNode : %u, itemCount : %u--> \n",lists[i].head,lists[i].tail,lists[i].currNode,lists[i].itemCount);
	}
}

void printOneList(List* Plist)
{
	unsigned int p = nodes[Plist->head].prevNode;
	int i = 1;
	printf("(%d) : %u --> \n",i,p);
	p = Plist->head;
	while( p != LIST_OOB_END)
	{
		printf("(%d) : %u, \"%d\" --> \n",i,p,*(int*)(nodes[p].itemP));
		++i;
		p = nodes[p].nextNode;
	}
}

int main()
{
	List* list1 = List_create();
	//Lets check empty list
	printf("%d\n",List_count(list1));
	if(List_first(list1) != NULL)
		printf("ERROR\n");
	if(List_last(list1) != NULL)
		printf("ERROR\n");
	if(List_next(list1) != NULL)
		printf("ERROR\n");
	if(List_prev(list1) != NULL)
		printf("ERROR\n");
	if(List_curr(list1) != NULL)
		printf("ERROR\n");
	if(List_trim(list1) != NULL)
		printf("ERROR\n");
	if(List_remove(list1) != NULL)
		printf("ERROR\n");

	int list1items[10] = {0,1,2,3,4,5,6,7,8,9};
	for(int i = 0;i < 10; i++)
		List_add(list1,&list1items[i]);
	//Printing the list(should be taking nodes 103 to 94)
	printOneList(list1);
	//Printing current and go back 5 steps and 1 step forward, then remove the item, go back two step and remove again
	printf("Current = %d \n",*(int*)(List_curr(list1)));
	for(int i = 0;i<5;i++)
		printf("Current = %d \n",*(int*)(List_prev(list1)));
	printf("Current = %d \n",*(int*)(List_next(list1)));
	printf("Removing = %d \n",*(int*)(List_remove(list1)));
	printf("Current = %d \n",*(int*)(List_prev(list1)));
	printf("Current = %d \n",*(int*)(List_prev(list1)));
	printf("Removing = %d \n",*(int*)(List_remove(list1)));
	//Print disjointed list
	printOneList(list1);
	printf("//////////////\n");
	//Add two items to the list , it should be non disjoint again
	List_add(list1,&list1items[3]);
	List_add(list1,&list1items[5]);
	//Print non disjointed list
	printOneList(list1);

	//Try creating 11 Lists
	return 0;
}