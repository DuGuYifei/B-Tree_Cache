#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct Node							//struct of node
{
	int _value;
	Node* xorPtr = 0;
};

typedef struct List							//information of list
{
	Node* _beg = 0;							//beginning
	Node* _end = 0;							//end
	Node* _act = 0;							//actual
};


Node* XORfunc(Node* node1, Node* node2)		//calculate xor value
{
	return (Node*)((unsigned long)(node1) ^ (unsigned long)(node2));
}


void NewList(List* list, Node* newNode)		//when nothing in the list create a new list(add the first element)
{
	list->_beg = newNode;
	list->_end = newNode;
	list->_act = newNode;
}


void DeleteBeg(List* list)					//delete the beginning
{
	Node* node = list->_beg;				//used to free the memory
	if (list->_beg == 0)					//if no element then free
		return;
	Node* second = XORfunc(0, XORfunc(0, list->_beg->xorPtr));		//next to the beginning
	if (second == nullptr)					//only one element in the list
	{
		list->_act = nullptr;
		list->_beg = nullptr;
		list->_end = nullptr;
	}
	else									//not only one element
	{
		if (list->_act == list->_beg)		//if actual = beginning,let actual change at the same time
			list->_act = list->_end;
		second->xorPtr = XORfunc(0, XORfunc(list->_beg, second->xorPtr));
		list->_beg = second;
	}
	free(node);								//free memory
}

void DeleteEnd(List* list)					//delete the end(similar to the delete beginning, but reverse all thing)
{
	Node* node = list->_end;
	if (list->_end == 0)
		return;
	Node* second = XORfunc(XORfunc(list->_end->xorPtr, 0), 0);
	if (second == nullptr)					//speecial situation
	{
		list->_act = nullptr;
		list->_beg = nullptr;
		list->_end = nullptr;
	}
	else									//ordinary situation
	{
		second->xorPtr = XORfunc(0, XORfunc(list->_end, second->xorPtr));
		if (list->_act == list->_end)
			list->_act = second;
		list->_end = second;
	}
	free(node);
}

void DeleteACT(List* list)					//delete the actual
{
	if (list->_act == 0)					//if no element in the lisst
		return;
	else if (list->_act == list->_beg)		//same with beginning
		DeleteBeg(list);
	else if (list->_act == list->_end)		//same with end
		DeleteEnd(list);
	else                                    //normal situation
	{
		Node* node = list->_act;			
		Node* temp1 = 0;					
		Node* temp2 = list->_beg;
		Node* temp = temp2;					//used to traverse the list to the actual
		while (temp != list->_act)
		{
			temp2 = XORfunc(temp1, temp2->xorPtr);
			temp1 = temp;
			temp = temp2;
		}

		Node* nextOfTemp2 = XORfunc(temp1, temp2->xorPtr);
		Node* nextOfNext = XORfunc(temp2, nextOfTemp2->xorPtr);
		nextOfTemp2->xorPtr = XORfunc(temp1, nextOfNext);

		if (temp1 == 0)					//if nothing before temp1
		{
			list->_act = list->_end;
			list->_beg = nextOfTemp2;
		}
		else                            //if one element before temp1 
		{
			Node* prevOfTemp1 = XORfunc(temp2, temp1->xorPtr);
			temp1->xorPtr = XORfunc(prevOfTemp1, nextOfTemp2);
			list->_act = temp1;
		}
		free(node);						//free memory
	}
}

void DeleteVal(List* list, int n)
{
	Node* temp1 = 0;					//traverse from beginning
	Node* temp2 = list->_beg;
	Node* temp = temp2;
	while (temp != 0)					//while not to the end
	{
		if (temp->_value == n)			//if value is what we want to delete
		{
			Node* node = temp;
			Node* nextOfTemp2 = XORfunc(temp1, temp2->xorPtr);
			if (temp == list->_act)		//three special situation
				DeleteACT(list);
			else if (temp == list->_beg)
				DeleteBeg(list);
			else if (temp == list->_end)
				DeleteEnd(list);

			else						//ordinary situation
			{
				Node* prevOfTemp1 = XORfunc(temp2, temp1->xorPtr);
				Node* nextOfNext = XORfunc(temp2, nextOfTemp2->xorPtr);
				temp1->xorPtr = XORfunc(prevOfTemp1, nextOfTemp2);
				nextOfTemp2->xorPtr = XORfunc(temp1, nextOfNext);
				free(node);
			}
			temp2 = nextOfTemp2;
		}
		else							//not delete and go to next one
		{
			temp2 = XORfunc(temp1, temp2->xorPtr);
			temp1 = temp;
		}
		temp = temp2;
	}
}


int main()
{

	char command[15];
	List list;

	while (scanf("%s", command) != EOF)
	{
		if (strcmp(command, "ACTUAL") == 0)				//print actual
		{
			if (list._act == 0)
				printf("NULL\n");
			else
				printf("%d\n", list._act->_value);
		}

		else if (strcmp(command, "NEXT") == 0)			//change actual to next
		{
			if (list._act == 0)
				printf("NULL\n");
			else
			{
				Node* temp1 = 0;
				Node* temp2 = list._beg;
				Node* temp = temp2;
				while (temp != list._act)				//find next
				{
					temp2 = XORfunc(temp1, temp2->xorPtr);
					temp1 = temp;
					temp = temp2;
				}

				list._act = XORfunc(temp1, temp2->xorPtr);	//change to the next
				if (list._act == 0)							//if it is the end
					list._act = list._beg;
				printf("%d\n", list._act->_value);
			}
		}

		else if (strcmp(command, "PREV") == 0)			//change actual to the previous
		{
			if (list._act == 0)
				printf("NULL\n");
			else
			{
				Node* temp1 = list._end;
				Node* temp2 = 0;
				Node* temp = temp1;
				while (temp != list._act)				//find previous
				{
					temp1 = XORfunc(temp1->xorPtr, temp2);
					temp2 = temp;
					temp = temp1;
				}

				list._act = XORfunc(temp1->xorPtr, temp2);	//change to the previous
				if (list._act == 0)							//if it is the first
					list._act = list._end;
				printf("%d\n", list._act->_value);
			}
		}

		else if (strcmp(command, "ADD_BEG") == 0)			//add element to the beginning
		{
			Node* newNode = (Node*)malloc(sizeof(Node));
			scanf("%d", &(newNode->_value));
			newNode->xorPtr = XORfunc(0, list._beg);
			if (list._beg == 0)								//if no element in list
			{
				NewList(&list, newNode);
			}
			else                                            //change the xor value of original beginning element
			{
				list._beg->xorPtr = XORfunc(newNode, XORfunc(0, list._beg->xorPtr));
				list._beg = newNode;
			}
		}

		else if (strcmp(command, "ADD_END") == 0)
		{
			Node* newNode = (Node*)malloc(sizeof(Node));
			scanf("%d", &(newNode->_value));
			newNode->xorPtr = XORfunc(list._end, 0);
			if (list._end == 0)								//if no element in list
			{
				NewList(&list, newNode);
			}
			else                                            //chenge thexor value of original end element
			{
				list._end->xorPtr = XORfunc(XORfunc(list._end->xorPtr, 0), newNode);
				list._end = newNode;
			}
		}

		else if (strcmp(command, "ADD_ACT") == 0)
		{

			Node* newNode = (Node*)malloc(sizeof(Node));
			scanf("%d", &(newNode->_value));
			if (list._act == 0)								//if no element in list
			{
				newNode->xorPtr = 0;
				NewList(&list, newNode);
			}
			else
			{
				Node* temp1 = list._end;
				Node* temp2 = 0;
				Node* temp = temp1;
				while (temp != list._act)					//traverse to the act, used to change the previous and original actual element's xor value
				{
					temp1 = XORfunc(temp1->xorPtr, temp2);
					temp2 = temp;
					temp = temp1;
				}

				temp = XORfunc(temp1->xorPtr, temp2);		//element before actual(temp1 is actual now)

				newNode->xorPtr = XORfunc(temp, temp1);
				list._act->xorPtr = XORfunc(newNode, temp2);
				if (temp != 0)								//if actual is not the first element
				{
					temp1 = XORfunc(temp->xorPtr, temp1);	//temp1 become before element of the one before actual
					temp->xorPtr = XORfunc(temp1, newNode);
				}
				else                                        //if actual is the beginning
				{
					list._beg = newNode;
				}
			}
		}
		else if (strcmp(command, "DEL_BEG") == 0)
		{
			DeleteBeg(&list);
		}

		else if (strcmp(command, "DEL_END") == 0)
		{
			DeleteEnd(&list);
		}

		else if (strcmp(command, "DEL_VAL") == 0)
		{
			int n;
			scanf("%d", &n);
			DeleteVal(&list, n);
		}

		else if (strcmp(command, "DEL_ACT") == 0)
		{
			DeleteACT(&list);
		}

		else if (strcmp(command, "PRINT_FORWARD") == 0)
		{
			Node* temp1 = 0;
			Node* temp2 = list._beg;
			Node* temp = temp2;
			while (temp != list._end)					//traverse from beginning and print
			{
				printf("%d ", temp->_value);
				temp2 = XORfunc(temp1, temp2->xorPtr);
				temp1 = temp;
				temp = temp2;
			}
			if (temp == 0)								//if no element
				printf("NULL\n");
			else                                        //the last element
				printf("%d\n", temp->_value);
		}

		else if (strcmp(command, "PRINT_BACKWARD") == 0)
		{
			Node* temp1 = list._end;
			Node* temp2 = 0;
			Node* temp = temp1;
			while (temp != list._beg)					//traverse from end to beginning
			{
				printf("%d ", temp->_value);
				temp1 = XORfunc(temp1->xorPtr, temp2);
				temp2 = temp;
				temp = temp1;
			}
			if (temp == 0)								//if no element
				printf("NULL\n");
			else                                        //the first element
				printf("%d\n", temp->_value);
		}
	}
	//free all memory in the list
	while (list._beg != 0)
		DeleteBeg(&list);
	return 0;
}