#include<stdio.h>
#include<stdlib.h>
#include<string.h>

/*****************
An implementation of an array-based bounded queue.
An array (bufferArray) contains pointers in memory to the Entry struct.  This array is initialized to be the maxsize of the queue.
The array is accessed in a circular fashion to add or remove entries from the queue, meaning that the first entry might not exist in the first bufferArray position
*****************/

struct Entry {
	int val;					// the value of the queue entry
	int pos;					// the position of the queue entry in the pointer array
};


int MAXSTORAGE;					// constant (in implementation) representing max size of the queue
struct Entry *first;			// pointer to the first queue entry
struct Entry *last;				// pointer to the last queue entry
void **bufferArray;				// array of pointers to entries

/**
	find the next array position given a queue entry
	@parameter item
	the entry to find the next array position for
	@return
	integer representing the next array position
**/
int getPositionAfterItem(struct Entry *item){
	int nextPos = 0;
	if (last != NULL){
		nextPos = item->pos + 1;
	}
	if (nextPos >= MAXSTORAGE){
		nextPos = 0;
	}
	return nextPos;
}

/**
	recursively print the queue contents
	@parameter item
	queue printing starts at this item
**/
void printQueue(struct Entry *item){
	if (item == first){
		printf("Queue is now ");
	}
	if (item != NULL){
		printf("%d ",item->val);
		if (item == last){
			printf("\n");
		}
		else{
			int nextPos = getPositionAfterItem(item);
			printQueue(bufferArray[nextPos]);
		}
	}
	else{
		printf("empty\n");
	}
}

/**
	get the next available array position (after the last entry), if the queue is not full
**/
int nextAvailableArrayPosition(){
	int nextPos = getPositionAfterItem(last);
	if (first != NULL && nextPos == first->pos){
		return -1;
	}
	return nextPos;
}

/**
	enqueue an item by finding the next available array position and creating an entry at that position.
	@param val
	value for the entry
	@returns Entry
	the added entry
**/
struct Entry *enqueue(int val){
	printf("Queueing %d\n", val);
	int position = nextAvailableArrayPosition();
	if (position >= 0){
		struct Entry *entry = bufferArray[position];

		entry->val = val;
		entry->pos = position;

		if (first == NULL){
			first = entry;
		}
		last = entry;

		printQueue(first);
		return entry;
	}
	else{
		printf("Queue size limit exceeded, ignoring entry.\n");
		return NULL;
	}
}

/**
	dequeue an item by setting the first item to be one after the first item
	@return
	the integer value of the dequeued entry
**/
int dequeue(){
	int dequeuedVal;
	printf("Dequeuing ");
	if (first != NULL){
		dequeuedVal = first->val;
		if (first != last){
			first = bufferArray[getPositionAfterItem(first)];
		}
		else{
			first = NULL;
		}
		printf("(%d)\n", dequeuedVal);
		printQueue(first);
	}
	else{
		//having to return an integer is not ideal because there's no concept of a null integer
		//just return a zero and assume the printf provides enough information
		dequeuedVal = 0;
		printf("\n");
		printf("Queue is empty.  Nothing to dequeue.\n");
	}
	return dequeuedVal;
}

/**
	initialize the buffer array depending on the max queue size
	@param size
	size of the queue
**/
void createQueueOfSize(int size){
	printf("Creating queue of size %d\n", size);
	MAXSTORAGE = size;
	bufferArray = malloc(size*sizeof(void*));
	for (int i = 0; i < size; ++i)
	{
		bufferArray[i] = malloc(sizeof(struct Entry));
	}
}

/**
	parse a file, moving line by line and calling the appropriate functions
**/
void parseFile(FILE *file){
	char line[256];
	while (fgets(line, sizeof(line), file)){
		//separate the line by spaces
		char *components = strtok(line, " ");
		if (strcmp(components,"size") == 0){
			createQueueOfSize(atoi(strtok(NULL, " ")));
		}
		else if (strcmp(components,"q") == 0){
			enqueue(atoi(strtok(NULL, " ")));
		}
		else if (strcmp(components,"d\n") == 0 || strcmp(components,"d") == 0){
			dequeue();
		}
		else{
			printf("Text file is not formatted correctly.  Unexpected input: %s\n",components);
			exit(0);
		}
	}
	fclose(file);
}

int main(int argc, char *argv[])
{
	if (argc == 2){
		char const* const fileName = argv[1];
		FILE* file = fopen(fileName, "r");
		if (file != NULL){
			parseFile(file);
		}
		else{
			printf("Please pass a valid config file as an argument.\n");
		}
	}
	else if (argc == 1){
		printf("Please pass config file as an argument.\n");
	}
	else if (argc > 2){
		printf("Too many arguments.\n");
	}
}

