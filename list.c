#include "list.h"
#include <stddef.h>
#include <stdio.h>

static Node nodes[LIST_MAX_NUM_NODES_OOB];
static List lists[LIST_MAX_NUM_HEADS];
static unsigned int removedNodes[LIST_MAX_NUM_NODES_OOB];
static unsigned int removedIndex = 0;
static unsigned int nodeArm = LIST_MAX_NUM_NODES_OOB - 1; // starts from last index
static bool listAvailable[LIST_MAX_NUM_HEADS];

// Makes a new, empty list, and returns its reference on success. 
// Returns a NULL pointer on failure.
List* List_create()
{
	for(int i = 0;i < LIST_MAX_NUM_HEADS;++i)
	{
		if(!listAvailable[i])
		{
			listAvailable[i] = true;
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
		return nodes[pList->head].itemP;
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
		return nodes[pList->tail].itemP;
	}
	else
	{
		nodes[pList->currNode].itemP = NULL;
		return NULL;
	}
}/*
// Advances pList's current item by one, and returns a pointer to the new current item.
// If this operation advances the current item beyond the end of the pList, a NULL pointer 
// is returned and the current item is set to be beyond end of pList.
void* List_next(List* pList)
{

}
// Backs up pList's current item by one, and returns a pointer to the new current item. 
// If this operation backs up the current item beyond the start of the pList, a NULL pointer 
// is returned and the current item is set to be before the start of pList.
void* List_prev(List* pList)
{
	
}
// Returns a pointer to the current item in pList.
void* List_curr(List* pList)
{
	
}*/
// Adds the new item to pList directly after the current item, and makes item the current item. 
// If the current pointer is before the start of the pList, the item is added at the start. If 
// the current pointer is beyond the end of the pList, the item is added at the end. 
// Returns 0 on success, -1 on failure.
int List_add(List* pList, void* pItem)
{
	//Handles removal backtracking(removedIndex 0 means non disjoint array, 0> disjoint)
	int NodeIndex;
	//Node array full
	if(removedIndex == 0 && nodeArm == 1)
	{
		printf("K1 %d \n",pList->itemCount);
		return -1;
	}
	//Not disjoint
	else if(removedIndex == 0)
	{
		NodeIndex = nodeArm--;
	}
	//Disjoint
	else
	{
		NodeIndex = removedNodes[removedIndex--];
	}
	printf("Going to write NodeIndex : %d \n",NodeIndex);
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
		//Current pointer at the last node(one head or last node)
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
/*
// Adds item to pList directly before the current item, and makes the new item the current one. 
// If the current pointer is before the start of the pList, the item is added at the start. 
// If the current pointer is beyond the end of the pList, the item is added at the end. 
// Returns 0 on success, -1 on failure.
int List_insert(List* pList, void* pItem)
{
	
}
// Adds item to the end of pList, and makes the new item the current one. 
// Returns 0 on success, -1 on failure.
int List_append(List* pList, void* pItem)
{
	
}
// Adds item to the front of pList, and makes the new item the current one. 
// Returns 0 on success, -1 on failure.
int List_prepend(List* pList, void* pItem)
{
	
}*/
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
		//Handles removal backtracking
		removedNodes[++removedIndex] = pList->currNode;

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
}/*
// Adds pList2 to the end of pList1. The current pointer is set to the current pointer of pList1. 
// pList2 no longer exists after the operation; its head is available
// for future operations.
void List_concat(List* pList1, List* pList2)
{
	
}
// Delete pList. pItemFreeFn is a pointer to a routine that frees an item. 
// It should be invoked (within List_free) as: (*pItemFreeFn)(itemToBeFreedFromNode);
// pList and all its nodes no longer exists after the operation; its head and nodes are 
// available for future operations.
typedef void (*FREE_FN)(void* pItem)
void List_free(List* pList, FREE_FN pItemFreeFn)
{
	
}
// Return last item and take it out of pList. Make the new last item the current one.
// Return NULL if pList is initially empty.
void* List_trim(List* pList)
{
	
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
typedef bool (*COMPARATOR_FN)(void* pItem, void* pComparisonArg)
void* List_search(List* pList, COMPARATOR_FN pComparator, void* pComparisonArg)
{
	
}
*/
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
	unsigned int head = Plist->head;
	printf("prev : %u,",nodes[head].prevNode);
	while( head != LIST_OOB_END)
	{
		printf("next : %u --> ",nodes[head].nextNode);
		head = nodes[head].nextNode;
	}
	printf("\n");
}
int main()
{
	List* mylist[LIST_MAX_NUM_HEADS];
	for(int i =0;i<LIST_MAX_NUM_HEADS;i++)
	{
		mylist[i] = List_create();
	}
	int a = 33;
	for(int i = 0;i<3;i++)
	{
		for(int x = 0;x<40;x++)
		List_add(mylist[i],&a);

	printOneList(mylist[i]);
	}

	//printAllNodes();
	//printAllLists();
//	for(int i = 0;i<103;i++)
//	{
//		int t = List_add(mylist,&a);
//		printf("%d \n ",*(unsigned int*)List_remove(mylist));
//	}
	return 0;
}