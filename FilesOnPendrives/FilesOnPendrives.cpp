#define _CRT_SECURE_NO_WARNINGS
#include<stdlib.h>
#include<stdio.h>
using namespace std;

typedef struct File
{
	int _id;        //id of file
	int _r;         //size of file
};

typedef struct Pendrive
{
	int _usedSize;	//size of having used
	int _numFile;	//how many files in it
	int* _filesID;	//files
};

//excange files position
void ExchangeFile(File* files, int left, int right)
{
	File medium = *(files + right);
	*(files + right) = *(files + left);
	*(files + left) = medium;
}
//used in quick sort the disk with r(size)
int SetPartR(File* files, int left, int right)
{
	File cmp = files[right];
	int standard = left;
	for (int i = left; i < right; i++)
	{
		if (cmp._r >= files[i]._r)
			ExchangeFile(files, i, standard++);
	}
	ExchangeFile(files, standard, right);
	return standard;
}
//sort disk with size
void SortR(File* files, int left, int right)
{
	if (left < right)
	{
		int standard = SetPartR(files, left, right);
		SortR(files, left, standard - 1);
		SortR(files, standard + 1, right);
	}
}

//exchange id of files in pendrive
void ExchangeID(int* filesID, int left, int right)
{
	int medium = *(filesID + right);
	*(filesID + right) = *(filesID + left);
	*(filesID + left) = medium;
}
//used in quick sort of pendrive
int SetPartID(int* filesID, int left, int right)
{
	int cmp = filesID[right];
	int standard = left;
	for (int i = left; i < right; i++)
	{
		if (cmp <= filesID[i])
			ExchangeID(filesID, i, standard++);
	}
	ExchangeID(filesID, standard, right);
	return standard;
}
//sort pendrive by ID
void SortID(int* filesID, int left, int right)
{
	if (left < right)
	{
		int standard = SetPartID(filesID, left, right);
		SortID(filesID, left, standard - 1);
		SortID(filesID, standard + 1, right);
	}
}

//insert files from disk to pendrive with smallest size(A then B)
void InsertPendrive(File* disk, Pendrive* pdA, Pendrive* pdB, int n, int p)
{
	if (n != 0)
	{
		pdA->_filesID = (int*)malloc((n) * sizeof(int));
		pdB->_filesID = (int*)malloc((n) * sizeof(int));
	}
	int i;
	for (i = 0; i < n; i++)
	{
		if ((disk + i)->_r + pdA->_usedSize <= p)
		{
			*(pdA->_filesID + i) = (disk + i)->_id;
			pdA->_usedSize += (disk + i)->_r;
		}
		else
		{
			pdA->_numFile = i;
			break;
		}
	}
	pdA->_filesID = (int*)realloc(pdA->_filesID, pdA->_numFile * sizeof(int));
	for (; i < n; i++)
	{
		if ((disk + i)->_r + pdB->_usedSize <= p)
		{
			*(pdB->_filesID + pdB->_numFile) = (disk + i)->_id;
			pdB->_numFile++;
			pdB->_usedSize += (disk + i)->_r;
		}
		else
		{
			pdB->_filesID = (int*)realloc(pdB->_filesID, pdB->_numFile * sizeof(int));
			return;
		}
	}
}

void OutputPendrive(Pendrive pd)
{
	for (int i = 0; i < pd._numFile; i++)
	{
		printf(" %d", *(pd._filesID + i));
	}
}

int main()
{
	int n;					//number of files
	int p;					//size of pendrive
	File* disk;				//all files in the disk
	Pendrive pendriveA;
	Pendrive pendriveB;
	pendriveA._numFile = 0;
	pendriveB._numFile = 0;
	pendriveA._usedSize = 0;
	pendriveB._usedSize = 0;

	scanf("%d %d", &n, &p);

	disk = (File*)malloc(n * sizeof(File));

	//input all files
	for (int i = 0; i < n; i++)
	{
		scanf("%d %d", &(disk + i)->_id, &(disk + i)->_r);
	}

	//sort files in disk by size
	SortR(disk, 0, n - 1);
	//Insert files into pendrive(choose smallest)
	InsertPendrive(disk, &pendriveA, &pendriveB, n, p);
	//sort files in pendrive by id
	SortID(pendriveA._filesID, 0, pendriveA._numFile - 1);
	SortID(pendriveB._filesID, 0, pendriveB._numFile - 1);

	//output result
	printf("%d\n1:", pendriveA._numFile + pendriveB._numFile);
	OutputPendrive(pendriveA);
	printf("\n2:");
	OutputPendrive(pendriveB);

	//free memory
	free(disk);
	if (pendriveA._usedSize != 0)
		free(pendriveA._filesID);
	if (pendriveB._usedSize != 0)
		free(pendriveB._filesID);

	return 0;
}