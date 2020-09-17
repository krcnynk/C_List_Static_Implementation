#include "list.h"
#include <stddef.h>
#include <stdio.h>
static Node nodes[LIST_MAX_NUM_NODES];
static List lists[LIST_MAX_NUM_HEADS];
static unsigned int usedListIndex = 0;
static unsigned int currNodeIndex = LIST_MAX_NUM_NODES - 1;
// Makes a new, empty list, and returns its reference on success. 
// Returns a NULL pointer on failure.
List* List_create()
{
	if(usedListIndex >= 10)
		return NULL;
	else
	{
		return &(lists[usedListIndex++]);
	}

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
}
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
	
}
// Adds the new item to pList directly after the current item, and makes item the current item. 
// If the current pointer is before the start of the pList, the item is added at the start. If 
// the current pointer is beyond the end of the pList, the item is added at the end. 
// Returns 0 on success, -1 on failure.
int List_add(List* pList, void* pItem)
{
	if(currNodeIndex == LIST_OOB_END)
	{
		return -1;
	}

	// If the list is empty
	if(pList->itemCount == 0)
	{
		pList->head = currNodeIndex;
		pList->tail = currNodeIndex;
		pList->currNode = currNodeIndex;
		++(pList->itemCount);

		nodes[pList->currNode].itemP = pItem;
		nodes[pList->currNode].nextNode = LIST_OOB_END;
		nodes[pList->currNode].prevNode = LIST_OOB_START;
	}

	else
	{
		--currNodeIndex;
		//Current pointer before start
		if(pList->currNode == LIST_OOB_START)
		{
			nodes[pList->head].prevNode = currNodeIndex;
			nodes[currNodeIndex].nextNode = pList->head;
			nodes[currNodeIndex].prevNode = LIST_OOB_START;
			nodes[currNodeIndex].itemP = pItem;
			pList->head = currNodeIndex;
			pList->currNode = currNodeIndex;
			++(pList->itemCount);
		}
		//Current pointer after end
		else if(pList->currNode == LIST_OOB_END)
		{
			nodes[pList->tail].nextNode = currNodeIndex;
			nodes[currNodeIndex].prevNode = pList->tail;
			nodes[currNodeIndex].nextNode = LIST_OOB_END;
			nodes[currNodeIndex].itemP = pItem;
			pList->tail = currNodeIndex;
			pList->currNode = currNodeIndex;
			++(pList->itemCount);
		}
		//Current pointer somewhere in middle
		else
		{
			unsigned int nextNodeTemp = nodes[pList->currNode].nextNode;
			//connect current node to inserted
			nodes[pList->currNode].nextNode = currNodeIndex;
			nodes[currNodeIndex].prevNode = pList->currNode;
			// connect inserted to to next node
			nodes[currNodeIndex].nextNode = nextNodeTemp;
			nodes[nextNodeTemp].prevNode = currNodeIndex;
			//change currNode to inserted
			pList->currNode = currNodeIndex;
			nodes[currNodeIndex].itemP = pItem;
		}
	}
	return 0;
}

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
	
}
// Return current item and take it out of pList. Make the next item the current one.
// If the current pointer is before the start of the pList, or beyond the end of the pList,
// then do not change the pList and return NULL.
void* List_remove(List* pList)
{
	if(pList->currNode == LIST_OOB_END || pList->currNode == LIST_OOB_START || pList->itemCount == 0)
	{
		return NULL;
	}
	else
	{
		if(pList->currNode == pList->head)
		{
			
		}
	}
}
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
int main()
{
	List* mylist = List_create();
	int a = 33;
	for(int i = 0;i<103;i++)
	{
		printf("%d \n ",List_add(mylist,&a));
	}

	return 0;
}