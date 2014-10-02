#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct Entry {
	int val;
	int pos;
};


int MAXSTORAGE;
struct Entry *first;
struct Entry *last;
void **bufferArray;

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

int nextAvailableArrayPosition(){
	int nextPos = getPositionAfterItem(last);
	if (first != NULL && nextPos == first->pos){
		printf("Queue size limit exceeded, ignoring entry.\n");
		return -1;
	}
	return nextPos;
}

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
		return NULL;
	}
}

void dequeue(){
	printf("Dequeuing\n");
	if (first != NULL){
		if (first != last){
			first = bufferArray[getPositionAfterItem(first)];
		}
		else{
			first = NULL;
		}
		printQueue(first);
	}
	else{
		printf("Queue is empty.  Nothing to dequeue.\n");
	}
}

void createQueueOfSize(int size){
	printf("Creating queue of size %d\n", size);
	MAXSTORAGE = size;
	bufferArray = malloc(size*sizeof(void*));
	for (int i = 0; i < size; ++i)
	{
		bufferArray[i] = malloc(sizeof(struct Entry));
	}
}

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
		else if (strcmp(components,"d\n") == 0){
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

