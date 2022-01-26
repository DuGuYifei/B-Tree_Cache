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

	Node* _front = 0;						//front of the queue
	Node* _nextFront = 0;					//next of the front
	Node* _back = 0;						//back of the queue
	Node* _nextBack = 0;					//next of the back

	int _sizeOfList = 0;					//size of the list,only be used in command SIZE to avoid traverse
	int _sizeOfQ = 0;						//size of the queue,only be used in command COUNT to avoid traverse

	bool backBehindFront = false;			//in the list, whether the back is behind front
};


Node* XORfunc(Node* node1, Node* node2)		//calculate xor value
{
	return (Node*)((unsigned long)(node1) ^ (unsigned long)(node2));
}

void NewList(List* list, Node* newNode)		//when nothing in the list create a new list(add the first element)
{
	list->_beg = newNode;
	list->_end = newNode;
}


//functions of list
void AddBeg(List* list)
{
	Node* newNode = (Node*)malloc(sizeof(Node));
	scanf("%d", &(newNode->_value));
	//newNode->xorPtr = XORfunc(0, list._beg);
	newNode->xorPtr = list->_beg;

	if (list->_beg == 0)				    //if no element in list
	{
		NewList(list, newNode);
	}
	else                                    //change the xor value of original beginning element
	{
		//list._beg->xorPtr = XORfunc(newNode, XORfunc(0, list._beg->xorPtr));
		list->_beg->xorPtr = XORfunc(newNode, list->_beg->xorPtr);
		list->_beg = newNode;
	}

	list->_sizeOfList++;

	if (list->backBehindFront)			//very special situation
		list->_sizeOfQ++;
}

void AddEnd(List* list)
{
	Node* newNode = (Node*)malloc(sizeof(Node));
	scanf("%d", &(newNode->_value));
	//newNode->xorPtr = XORfunc(list._end, 0);
	newNode->xorPtr = list->_end;

	if (list->_end == 0)					//if no element in list
	{
		NewList(list, newNode);
	}
	else                                    //change the xor value of original end element
	{
		if (list->_front == list->_end)
			list->_nextFront = newNode;
		if (list->_back == list->_end)
			list->_nextBack = newNode;

		//list._end->xorPtr = XORfunc(XORfunc(list._end->xorPtr, 0), newNode);
		list->_end->xorPtr = XORfunc(list->_end->xorPtr, newNode);
		list->_end = newNode;
	}

	list->_sizeOfList++;

	if (list->backBehindFront)			//very special situation
		list->_sizeOfQ++;
}

void DeleteBeg(List* list)					//delete the beginning
{
	Node* node = list->_beg;				//used to free the memory
	if (list->_beg == 0)					//if no element
		return;

	if (list->backBehindFront)
		list->_sizeOfQ--;
	if (list->_beg == list->_back)			//beginning = back of queue
	{
		list->_back = list->_back->xorPtr;
		list->_sizeOfQ--;
	}
	else if (list->_beg == list->_front)	//beginning = front of queue
	{
		list->_front = list->_end;
		list->_nextFront = 0;
		list->backBehindFront = false;
	}

	Node* second = list->_beg->xorPtr;		//next to the beginning
	if (second == nullptr)					//only one element in the list
	{
		list->_beg = nullptr;
		list->_end = nullptr;
		list->_front = nullptr;
		list->_back = nullptr;
	}
	else									//not only one element
	{
		//second->xorPtr = XORfunc(0, XORfunc(list->_beg, second->xorPtr));
		second->xorPtr = XORfunc(list->_beg, second->xorPtr);
		list->_beg = second;
	}
	free(node);								//free memory
	
	list->_sizeOfList--;
}

void DeleteEnd(List* list)					//delete the end(similar to the delete beginning, but reverse all thing)
{
	Node* node = list->_end;
	if (list->_end == 0)
		return;

	if (list->backBehindFront)
		list->_sizeOfQ--;
	if (list->_end == list->_front)			//end = front of queue
	{
		list->_front = list->_front->xorPtr;
		list->_sizeOfQ--;
	}
	else if (list->_end == list->_back)		//end = back of queue
	{
		list->_back = list->_beg;
		list->_nextBack = list->_beg->xorPtr;
		list->backBehindFront = false;
	}

	Node* second = list->_end->xorPtr;
	if (second == nullptr)					//speecial situation
	{
		list->_beg = nullptr;
		list->_end = nullptr;
		list->_front = nullptr;
		list->_back = nullptr;
	}
	else									//ordinary situation
	{
		//second->xorPtr = XORfunc(0, XORfunc(list->_end, second->xorPtr));
		second->xorPtr = XORfunc(list->_end, second->xorPtr);
		list->_end = second;
	}
	free(node);

	list->_sizeOfList--;
}

void PrintForward(List list)
{
	Node* temp1 = 0;
	Node* temp2 = list._beg;
	Node* temp = temp2;
	while (temp != list._end)			    //traverse from beginning and print
	{
		printf("%d ", temp->_value);
		temp2 = XORfunc(temp1, temp2->xorPtr);
		temp1 = temp;
		temp = temp2;
	}
	if (temp == 0)							//if no element
		printf("NULL\n");
	else                                    //the last element
		printf("%d\n", temp->_value);
}

void PrintBackward(List list)
{
	Node* temp1 = list._end;
	Node* temp2 = 0;
	Node* temp = temp1;
	while (temp != list._beg)				//traverse from end to beginning
	{
		printf("%d ", temp->_value);
		temp1 = XORfunc(temp1->xorPtr, temp2);
		temp2 = temp;
		temp = temp1;
	}
	if (temp == 0)							//if no element
		printf("NULL\n");
	else                                    //the first element
		printf("%d\n", temp->_value);
}


//function of queue

//void Count(List* list)
//{
//	if (list->_front == 0) {				//no element
//		list->_sizeOfQ = 0;
//		return;
//	}
//	if (list->_front == list->_back) {		//only one element
//		list->_sizeOfQ = 1;
//		return;
//	}
//
//	Node* temp1 = list->_front;
//	Node* temp2 = list->_nextFront;
//	Node* temp = temp1;
//	list->_sizeOfQ = 1;
//	while (temp != list->_back)				//traverse from front to back
//	{
//		list->_sizeOfQ++;
//		temp1 = XORfunc(temp1->xorPtr, temp2);
//		temp2 = temp;
//		temp = temp1;
//	}
//}

void Push(List* list)
{
	if (list->_sizeOfList == list->_sizeOfQ)//if there is no element can be used in the list, create a new element
	{
		if (!list->backBehindFront)			//if back of queue is the beginning of list
		{
			AddBeg(list);
			list->_nextBack = list->_back;
			list->_back = list->_beg;
			if (list->_front == 0)			//if the queue is empty
			{
				list->_front = list->_beg;
				list->_nextFront = 0;
			}
		}
		else								//add node between front and back												
		{
			list->_sizeOfList++;
			Node* newNode = (Node*)malloc(sizeof(Node));
			scanf("%d", &(newNode->_value));
			newNode->xorPtr = XORfunc(list->_front, list->_back);
			list->_front->xorPtr = XORfunc(XORfunc(list->_front->xorPtr, list->_nextFront), newNode);
			list->_nextFront = newNode;
			list->_back->xorPtr = XORfunc(newNode, list->_nextBack);
			list->_nextBack = list->_back;
			list->_back = newNode;
		}
	}
	else									//if there is element in list can be used
	{
		if (list->_front == 0)				//if queue is empty
		{
			scanf("%d", &(list->_end->_value));
			list->_front = list->_end;
			list->_back = list->_end;
			list->_nextFront = 0;
			list->_nextBack = 0;
		}
		else {
			Node* temp = XORfunc(list->_back->xorPtr, list->_nextBack);
			if (temp != 0)					//if the element before back is not left side
			{
				if (temp == list->_front){					//this is very special situation
					list->_sizeOfList++;
					Node* newNode = (Node*)malloc(sizeof(Node));
					scanf("%d", &(newNode->_value));
					newNode->xorPtr = XORfunc(list->_front, list->_back);
					list->_front->xorPtr = XORfunc(XORfunc(list->_front->xorPtr, list->_nextFront), newNode);
					list->_nextFront = newNode;
					list->_back->xorPtr = XORfunc(newNode, list->_nextBack);
					list->_nextBack = list->_back;
					list->_back = newNode;
				}
				else {
					scanf("%d", &(temp->_value));
					list->_nextBack = list->_back;
					list->_back = temp;
				}
			}
			else								//if the element before back is left side, it should go to the end
			{
				temp = list->_end;
				scanf("%d", &(temp->_value));
				list->_back = list->_end;
				list->_nextBack = 0;
				list->backBehindFront = true;
			}
		}
	}
	list->_sizeOfQ++;
}

void Pop(List* list)
{
	if (list->_front == 0)					//no element in queue
		printf("NULL\n");
	else {
		printf("%d\n", list->_front->_value);
		list->_sizeOfQ--;

		if (list->_front == list->_back)	//front = back
		{
			list->_front = 0;
			list->_back = 0;
		}
		else
		{
			Node* temp = list->_front;
			list->_front = XORfunc(list->_front->xorPtr, list->_nextFront);	//let front be the element before it
			list->_nextFront = temp;
			
			if (list->_front == 0)			//if the element before front is 0, then go to the end
			{
				list->_front = list->_end;
				list->_nextFront = 0;
				list->backBehindFront = false;
			}
		}
	}
}

void PrintQ(List list)
{
	Node* temp1 = list._front;
	Node* temp2 = list._nextFront;
	Node* temp = temp1;

	while (temp != list._back)				//traverse from front to back
	{
		if (temp1 == 0)
		{
			temp1 = list._end;
			temp2 = 0;
			temp = temp1;
			continue;
		}
		printf("%d ", temp->_value);
		temp1 = XORfunc(temp1->xorPtr, temp2);
		temp2 = temp;
		temp = temp1;
	}
	if (temp == 0)							//if no element in queue
		printf("NULL\n");
	else                                    //the last one
		printf("%d\n", temp->_value);
}

void Garbage(List *list, bool soft)			//bool soft choose soft or hard pattern
{
	if (list->_sizeOfList == list->_sizeOfQ)
		return;

	Node* temp1 = list->_front;
	Node* temp2 = list->_nextFront;
	Node* temp = temp2;
	Node* prevOfFront = 0;
	if (list->_front == 0)					//if queue is empty, then change all element in the list
	{
		temp1 = list->_beg;
		temp2 = list->_beg->xorPtr;
		temp = temp2;
	}
	else									//if queue is not empty, then we need use the xor pointer of front and back
		prevOfFront = XORfunc(list->_front->xorPtr, list->_nextFront);

	while (temp != list->_back)				//traverse from front to back as sequece of list(if no element in queue, the back will be nullptr which is the same with rigth side)
	{
		Node* node = temp1;					//used to free memory
		if (temp == 0){
			temp1 = 0;
			temp2 = list->_beg;
			temp = temp2;
		}
		else {
			temp2 = XORfunc(temp1, temp2->xorPtr);
			temp1 = temp;
			temp = temp2;
		}

		if (soft && node != list->_front && node != 0)				//if garbage soft
			node->_value = 0;
		else if (!soft && node != list->_front && node != 0)		//if garbage hard
		{
			free(node);
			list->_sizeOfList--;
		}
	}
	//last element
	Node* node = temp1;							//used to free memory
	if (soft && node != list->_front && node != 0)					//if garbage soft
		node->_value = 0;
	else if (!soft && node != list->_front && node != 0)			//if garbage hard
	{
		if (list->_sizeOfList == 1)
		{
			list->_beg = 0;
			list->_end = 0;
		}
		free(node);
		list->_sizeOfList--;
	}

	if (!soft)									//if hard pattern, then change the front and back xor pointer
	{
		//after garbage hard, there will only be 3 situation:
		//1.0 front/back 0. do nothing
		if (list->_back == list->_front)
		{
			list->_beg = list->_front;
			list->_end = list->_front;
			list->_nextFront = 0;
			list->_nextBack = 0;
		}
		//2.0 back ... front 0
		else if (!list->backBehindFront)
		{
			//list->_back->xorPtr = XORfunc(0, list->_nextBack);
			//list->_front->xorPtr = XORfunc(0, prevOfFront);
			list->_back->xorPtr = list->_nextBack;
			list->_front->xorPtr = prevOfFront;
			list->_beg = list->_back;
			list->_end = list->_front;
			list->_nextFront = 0;
		}
		//3. ...front back ...
		else
		{
			list->_back->xorPtr = XORfunc(list->_front, list->_nextBack);
			list->_front->xorPtr = XORfunc(prevOfFront, list->_back);
			list->_nextFront = list->_back;
		}
	}
}


int main()
{

	char command[15];
	List list;

	while (scanf("%s", command) != EOF)
	{
		if (strcmp(command, "ADD_BEG") == 0)					//add element to the beginning of list
			AddBeg(&list);

		else if (strcmp(command, "ADD_END") == 0)				//add element to the end of list
			AddEnd(&list);

		else if (strcmp(command, "DEL_BEG") == 0)				//delete the element of beginning of list
			DeleteBeg(&list);

		else if (strcmp(command, "DEL_END") == 0)				//delete the element of end of list
			DeleteEnd(&list);

		else if (strcmp(command, "PRINT_FORWARD") == 0)			//print the list from begenning to the end
			PrintForward(list);

		else if (strcmp(command, "PRINT_BACKWARD") == 0)		//print the list from end to beginning
			PrintBackward(list);

		else if (strcmp(command, "SIZE") == 0)					//size of list
			printf("%d\n", list._sizeOfList);

		else if (strcmp(command, "PUSH") == 0)					//ass a new element in queue
			Push(&list);

		else if (strcmp(command, "POP") == 0)					//remove the first element in queue
			Pop(&list);

		else if (strcmp(command, "PRINT_QUEUE") == 0)			//print queue
			PrintQ(list);

		else if (strcmp(command, "COUNT") == 0)					//size of queue
			//Count(&list);
			printf("%d\n", list._sizeOfQ);

		else if (strcmp(command, "GARBAGE_SOFT") == 0)			//soft garbage, change value to 0
			Garbage(&list, true);

		else if (strcmp(command, "GARBAGE_HARD") == 0)			//hard garbage, delete the addtional element
			Garbage(&list, false);
	}

	//free all memory in the list
	while (list._beg != 0)
		DeleteBeg(&list);
	return 0;
}