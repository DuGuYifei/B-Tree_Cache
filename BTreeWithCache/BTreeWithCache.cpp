#include <iostream>
#include <string>

//a tree of order t, at least t-1 elements in each node (except the root), at most 2t-1.
typedef struct Node
{
	int keyNum;						//quantity of key in it
	int* key;						//the keys stored in this node
	Node** children;				//children
	bool isLeaf;					//whether be the leaf node

	Node(bool isleaf, int order)
	{
		isLeaf = isleaf;
		key = new int[2 * order - 1];
		children = new Node * [2 * order];
		keyNum = 0;
	}
};


typedef struct Cache
{
	int keyNum = 0;
	int* key;
	int* depth;
};


//-----Add--------------------------------------------------------------------------
void Split(Node*& parent, Node*& split, int order, int position)
{
	//create a new node on its right to store t-1 keys
	Node* splitRight = new Node(split->isLeaf, order);
	splitRight->keyNum = order - 1;

	split->keyNum = order - 1;

	//make the last (t-1) keys to the new node
	for (int i = 0; i < order - 1; i++)
		splitRight->key[i] = split->key[i + order];

	//make the last children to new node
	if (!split->isLeaf)
		for (int i = 0; i < order; i++)
			splitRight->children[i] = split->children[order + i];

	//spare a space for new child (n keys means n+1 children)
	for (int i = parent->keyNum; i > position; i--)
		parent->children[i + 1] = parent->children[i];

	parent->children[position + 1] = splitRight;

	//spare a space for new key
	for (int i = parent->keyNum - 1; i >= position; i--)
		parent->key[i + 1] = parent->key[i];

	parent->key[position] = split->key[order-1];
	parent->keyNum++;
}

void InsertNonFull(Node* node, int k,int order)
{
	//if this node is leaf, spare a space for new key
	int i = node->keyNum - 1;//i is index of the last element of the key array
	if (node->isLeaf)
	{
		for (i;i >= 0 && node->key[i] > k; i--)
			node->key[i + 1] = node->key[i];

		node->key[i + 1] = k;
		node->keyNum++;
	}

	//if this node is not leaf
	else
	{
		//find one of its children to store the new key(Recursion to find the node)
		for (i; i >= 0 && node->key[i] > k; i--);
		//now i is the index before the position

		//if the child is full
		if (node->children[i + 1]->keyNum == 2 * order - 1)
		{
			Split(node, node->children[i + 1], order, i + 1);
			//now there is a new key from its children, compare it with k
			//if it is smaller than k, then k should add into splitRight(new child which is splited out)
			if (node->key[i + 1] < k)
				i++;
		}

		InsertNonFull(node->children[i + 1], k, order);
	}
}

void Insert(int k, Node*& root,int order)
{
	//if root is null, create a new B-tree.
	if (root == NULL)
	{
		root = new Node(true, order);
		root->key[0] = k;
		root->keyNum = 1;
		return;
	}

	//if root is full, create a new root
	if (root->keyNum == 2 * order - 1)
	{
		Node* newRoot = new Node(false, order);
		newRoot->children[0] = root;
		Split(newRoot, root, order, 0);

		//find position that can insert new key
		if (newRoot->key[0] < k)	//compare with first key come from old root after split
			InsertNonFull(newRoot->children[1], k, order);

		else
			InsertNonFull(newRoot->children[0], k, order);

		root = newRoot;
	}
	//if root is not full
	else
		InsertNonFull(root, k, order);

}


//-----Search----------------------------------------------------------
void Search(int k,int order,Node* root)
{
	int i = 0;						//index of the key

	//find the position that k is smaller than or equal to this position
	for (i; i<root->keyNum && k>root->key[i]; i++);
	
	if (root->key[i] == k			//if equal
		&& i < root->keyNum)		//in case that the next key which is out of keyNum is equal to the search key
	{
		std::cout << k << " +" << std::endl;
		return;
	}

	if (root->isLeaf)				//if smaller but not have children
	{
		std::cout << k << " -" << std::endl;
		return;
	}

	Search(k, order, root->children[i]);//if smaller but have children
}


//-----Print-----------------------------------------------------------
void Print(Node* root)
{
	int i = 0;
	for (i; i < root->keyNum; i++)
	{
		//print its child firstly, the last child is bigger, so it should print lately
		//the children number is bigger than key number, 
		//so I need print the last child individually
		if (!root->isLeaf)
			Print(root->children[i]);

		std::cout << root->key[i]<<" ";
	}

	//print the last child which is bigger than the key in the parent node
	if (!root->isLeaf)
		Print(root->children[i]);
}


//-----Save-------------------------------------------------------------
void Save(std::string* str, Node* root)	//the same principle with Print()
{
	*str += "( ";
	int i = 0;
	for (i; i < root->keyNum; i++)
	{
		//print its child firstly, the last child is bigger, so it should print lately
		//the children number is bigger than key number, 
		//so I need print the last child individually
		if (!root->isLeaf)
			Save(str, root->children[i]);

		*str += std::to_string(root->key[i]);
		*str += " ";
	}

	//print the last child which is bigger than the key in the parent node
	if (!root->isLeaf)
		Save(str, root->children[i]);

	*str += ") ";
}


//-----Load--------------------------------------------------------------
void Load(Node*& root, int order)
{
	bool startLoad = true;					//after finish the load of this node, it will be false

	root = new Node(true, order);			//new node

	std::string str = "";					//what we input

	while (startLoad)
	{
		std::cin >> str;

		if (str == "(")						// "(" means there is child in this node
		{
			root->isLeaf = false;
			Load(root->children[root->keyNum], order);
		}

		else if (str == ")")				// ")" means we finish this node
		{
			startLoad = false;
		}

		else								//else is number which is the key in this node
		{
			int key = stoi(str);
			root->key[root->keyNum] = key;
			root->keyNum++;
		}
	}
	
}


//-----Remove------------------------------------------------------------
int GetPrev(int position, Node* root)
{
	Node* temp = root->children[position];
	//if not the leaf node, go to the last child
	while (!temp->isLeaf)
		temp = temp->children[temp->keyNum];

	//return the last key of previous node
	return temp->key[temp->keyNum - 1];
}

int GetNext(int position, Node* root)
{
	Node* temp = root->children[position + 1];
	//if not the leaf node, go to the first child
	while (!temp->isLeaf)
		temp = temp->children[0];

	//return the first key of previous node
	return temp->key[0];
}

void Merge(int position, int order, Node*& root)
{
	Node* child1 = root->children[position];
	Node* child2 = root->children[position + 1];

	//firstly add the k into left child
	child1->key[order - 1] = root->key[position];

	//secondly add the keys of right child into left child
	for (int i = 0; i < child2->keyNum; i++)
		child1->key[order + i] = child2->key[i];

	//if it is not the leaf node, it should add the children of right child to leaft child
	if (!child1->isLeaf)
		for (int i = 0; i <= child2->keyNum; i++)
			child1->children[order + i] = child2->children[i];

	child1->keyNum += (1 + child2->keyNum);

	//delete the k in the root, and delete the right child
	for (int i = position + 1; i < root->keyNum; i++)
		root->key[i - 1] = root->key[i];

	for (int i = position + 2; i <= root->keyNum; i++)
		root->children[i - 1] = root->children[i];

	root->keyNum--;

	delete(child2);
}

void Fill(int position,int order,Node*& node)
{
	//if prev child have keys more than t-1
	if (position != 0 && node->children[position - 1]->keyNum >= order)
	{
		Node* child1 = node->children[position];
		Node* child2 = node->children[position - 1];

		//spare the first space of key array for the key from node(parent)
		for (int i = child1->keyNum - 1; i >= 0; i--)
			child1->key[i + 1] = child1->key[i];

		child1->key[0] = node->key[position - 1];

		//spare the first space of children array for the child from previous
		if (!child1->isLeaf)
		{
			for (int i = child1->keyNum; i >= 0; i--)
				child1->children[i + 1] = child1->children[i];

			child1->children[0] = child2->children[child2->keyNum];
		}

		//let the last key of previous child go up to node(parent)
		node->key[position - 1] = child2->key[child2->keyNum - 1];

		child1->keyNum++;
		child2->keyNum--;
	}
	//if next child have keys more than t-1
	else if (position != node->keyNum && node->children[position + 1]->keyNum >= order)
	{
		Node* child1 = node->children[position];
		Node* child2 = node->children[position + 1];

		//insert the key from node(parent) to child1, insert the key from child2 to node(parent)
		child1->key[child1->keyNum] = node->key[position];
		node->key[position] = child2->key[0];

		if (!child1->isLeaf)
		{
			//insert the first child from next brother
			child1->children[child1->keyNum + 1] = child2->children[0];

			//adjust the children array in next brother
			for (int i = 1; i <= child2->keyNum; i++)
				child2->children[i - 1] = child2->children[i];
		}

		//adjust the key array in child2
		for (int i = 1; i < child2->keyNum; i++)
			child2->key[i - 1] = child2->key[i];

		child1->keyNum++;
		child2->keyNum--;
	}
	//else both have keys t-1
	else
	{
		if (position != node->keyNum)
			Merge(position, order, node);
		else
			Merge(position - 1, order, node);
	}
}

void Remove(int k, int order,Node*& root)
{
	//find the position of k (equal or smaller than tis position)
	int i = 0;
	for (i; i < root->keyNum && root->key[i] < k; i++);
	//if it has find to the last key,then the position will be Num, i.e. the index of last key + 1
	//and it is also the index of the child (which might contain k)

	//if find the key
	if (i < root->keyNum && root->key[i] == k)
	{
		//if it is the leaf node, delete the key directly
		if (root->isLeaf)
		{
			for (i = i + 1; i < root->keyNum; i++)
				root->key[i - 1] = root->key[i];
			root->keyNum--;
		}

		//if it is not the leaf node
		else
		{
			//if previous' keynumber bigger than t-1, the find the last previous key
			if (root->children[i]->keyNum >= order)
			{
				root->key[i] = GetPrev(i, root);
				Remove(root->key[i], order, root->children[i]);
			}

			//otherwise if the next children have key more than t-1, find the first next key
			else if (root->children[i + 1]->keyNum >= order)
			{
				root->key[i] = GetNext(i, root);
				Remove(root->key[i], order, root->children[i + 1]);
			}

			//the last situation is both prev and next have t-1 keys
			else
			{
				Merge(i, order, root);
				//after merge, there is the k in the left child, so we need remove from it
				Remove(k, order, root->children[i]);
			}
		}
	}
	//if not find the key in this root
	else
	{
		//if it has been the leaf node, means there is no this key in the B-tree
		if (root->isLeaf)
			return;

		//In case that when delete the key, it may cause the node less than t-1.
		//We can solve it by fill the node before delete,
		// or fill after delete. Here is fill before delete.
		//Before fill, we need to store a sign which will be used later to get information whether we make a fill procedure
		bool i_keyNum = (i == root->keyNum);
		
		if (root->children[i]->keyNum < order)
			Fill(i, order, root);

		//if it is not leaf node,continue searching
		if (i > root->keyNum && i_keyNum)	//we have had a fill procedure just now, which may change the keys in the node by merge
			Remove(k, order, root->children[i - 1]);
		else
			Remove(k, order, root->children[i]);
	}
}


//-----MyCache--------------------------------------------------------------------------
//when want to store new key, it will free the key which used least and in lowest depth.

typedef struct CacheKey
{
	int key;
	int depth;
	int useTime = 0;
};

bool Compare_useTime_depth(CacheKey x, CacheKey y)
{
	if (x.useTime != y.useTime)
	{
		return x.useTime <= y.useTime;
	}
	else
	{
		return x.depth <= y.depth;
	}
}

int getStandard(CacheKey* array, int i, int j) {
	CacheKey key = array[i];
	while (i < j)
	{
		while (i < j && Compare_useTime_depth(array[j], key))
			j--;

		if (i < j)
			array[i] = array[j];

		while (i < j && Compare_useTime_depth(key, array[i]))
			i++;

		if (i < j)
			array[j] = array[i];
	}
	array[i] = key;
	return i;
}

void QuickSort(CacheKey* array, int low, int high) {
	if (low < high) {
		int standard = getStandard(array, low, high);
		QuickSort(array, low, standard - 1);
		QuickSort(array, standard + 1, high);
	}
}

int ImpactOfCache_2(Cache* cache, int size)
{
	//CacheKey array
	CacheKey* cacheArray = new CacheKey[size];

	int measure = 0;

	for (int i = 0; i < size && i < cache->keyNum; i++)
	{
		cacheArray[i].key = cache->key[i];
		cacheArray[i].depth = cache->depth[i];
		cacheArray[i].useTime++;
		measure += cache->depth[i];
	}

	for (int i = size; i < cache->keyNum; i++)
	{
		QuickSort(cacheArray, 0, size - 1);

		//search the key in the cache array
		int j = 0;
		for (j; j < size; j++)
			if (cacheArray[j].key == cache->key[i])
			{
				cacheArray[j].useTime++;
				j = size + 1;
			}

		//if not find the key
		if (j <= size)
		{
			//insert the new key into cache.
			//replace the one which used least and lowest depth
			cacheArray[size - 1].key = cache->key[i];
			cacheArray[size - 1].depth = cache->depth[i];
			cacheArray[size - 1].useTime = 1;

			//measure the depth
			measure += cache->depth[i];
		}
	}

	delete(cacheArray);

	return measure;
}


//-----Cache------------------------------------------------------------------------------
void CacheSearch(int k, int order, Node* root, int* depth)		//same principle with search
{
	int i = 0;						//index of the key

	(*depth)++;

	//find the position that k is smaller than or equal to this position
	for (i; i<root->keyNum && k>root->key[i]; i++);

	if (root->key[i] == k)			//if equal,find the key
	{
		return;
	}

	if (root->isLeaf)				//if smaller but not have children, the key does not exist
	{
		return;
	}

	CacheSearch(k, order, root->children[i], depth);//if smaller but have children
}

int ImpactOfCache(Cache* cache, int size)
{
	//FIFO array
	int* cacheArray = new int[size];

	int measure = 0;

	int i = 0;
	for (i; i < size && i < cache->keyNum; i++)
	{
		cacheArray[i] = cache->key[i];
		measure += cache->depth[i];
	}

	for (int i = size; i < cache->keyNum; i++)
	{
		//search the key in the cache array
		int j = 0;
		for (j; j < size; j++)
			if (cacheArray[j] == cache->key[i])
				j = size + 1;

		//if not find the key
		if (j <= size)
		{
			//FIFO
			for (int k = 1; k < size; k++)
				cacheArray[k - 1] = cacheArray[k];

			//insert the new key into cache
			cacheArray[size - 1] = cache->key[i];

			//measure the depth
			measure += cache->depth[i];
		}
	}

	delete[]cacheArray;

	return measure;
}

void CalculateCache(Cache* cache, int size, int order, Node* root)
{
	std::cout << "NO ";

	int measure = 0;				//measure the sum of depth

	//search every key and store their depth. Then add into the sum of depth
	for (int i = 0; i < cache->keyNum; i++)
	{
		cache->depth[i] = 0;

		CacheSearch(cache->key[i], order, root, &(cache->depth[i]));

		measure += cache->depth[i];
	}

	std::cout << "CACHE: " << measure << " ";

	//measure the impact of cache
	measure = ImpactOfCache_2(cache, size);
	std::cout << "CACHE: " << measure <<std::endl;
}

void Calculate(int order, Node* root)
{
	Cache cache;
	int size;

	std::cin >> size;
	cache.key = (int*)malloc(sizeof(int));

	while (getchar() != '\n')
	{
		cache.keyNum++;
		cache.key = (int*)realloc(cache.key, cache.keyNum * sizeof(int));
		std::cin >> cache.key[cache.keyNum - 1];
	}
	cache.depth = (int*)malloc(cache.keyNum * sizeof(int));

	CalculateCache(&cache, size, order, root);

	free(cache.key);
	free(cache.depth);
}


void DeleteNode(Node*& node)
{
	if (node != NULL)
	{
		delete(node->key);//[]
		delete(node->children);
		delete node;
		node = NULL;
	}
}

void ClearMemory(Node*& node)
{
	if (node != NULL)
	{
		if (!node->isLeaf)
		{
			//clear its children firstly
			for (int i = 0; i <= node->keyNum; i++)
				ClearMemory(node->children[i]);
		}
		//then clear itself
		DeleteNode(node);
	}
}


int main()
{
	char command = ' ';

	int order;						//the order of the tree;

	int key;

	std::string str;

	Node* root = nullptr;

	Cache cache;

	while (command != 'X')
	{
		std::cin >> command;
		switch (command)
		{
		case 'I':

			std::cin >> order;
			
			break;

		case 'A':

			std::cin >> key;
			Insert(key, root, order);
			
			break;

		case '?':

			std::cin >> key;
			Search(key, order, root);
			
			break;

		case 'P':
			
			Print(root);
			std::cout << std::endl;
			
			break;

		case 'L':
			
			std::cin >> order >> str;	//str here is used to store the first bracket

			delete(root);
			Load(root, order);
			
			break;

		case 'S':
			//if (root != NULL)
			//{
				str = "";
				Save(&str, root);

				std::cout << order << std::endl;
				std::cout << str << std::endl;
			//}
			break;

		case 'R':

			std::cin >> key;

			Remove(key, order, root);

			//adjust the root, in case that after merge, the root has no key
			if (root->keyNum == 0)
			{
				Node* temp = root;
				if (root->isLeaf)
					root = NULL;
				else
					root = root->children[0];
				DeleteNode(temp);
			}

			break;

		case 'C':

			Calculate(order, root);

			break;
		}
	}

	ClearMemory(root);

	return 0;
}