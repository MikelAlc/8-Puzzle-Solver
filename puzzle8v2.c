#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define HASHTABLE_SIZE 20011 

struct HeapNode{
    char pos[10];
    int priority; 
    long int parentIndex;
};

struct Heap{
    struct HeapNode heap[181440];
    long int tail;
};

struct HashTable{
    long int count;
    char arr[HASHTABLE_SIZE][10];
    long int parent[HASHTABLE_SIZE];
};

/*Assumes a valid position is given*/
void printPosition(char *pos)
{
    int i;
    for(i=0;i<9;i++)
    {
        printf("%c",*pos);
        pos++;
        if((i+1)%3 == 0) putchar('\n');

    }
    putchar('\n');
}

void swapHeapNodes(struct HeapNode *node1, struct HeapNode *node2){
    struct HeapNode temp = *node1;
    *node1 = *node2;
    *node2 = temp; 
}

void perculateDown(struct Heap * fringe, long int index){
    long int smallest = index;
    long int left = 2 * index + 1;
    long int right = 2 * index + 2;
    if (left <= fringe->tail && fringe->heap[left].priority < fringe->heap[smallest].priority)
        smallest=left;
    if (right <= fringe->tail && fringe->heap[right].priority < fringe->heap[smallest].priority)  
        smallest = right;
    if (smallest != index){
        swapHeapNodes(&fringe->heap[index], &fringe->heap[smallest]);
        perculateDown(fringe, smallest);
    }
}  

void perculateUp(struct Heap *fringe, long int index){
    long int parent = (index - 1) / 2;
    if (parent > -1 && fringe->heap[index].priority < fringe->heap[parent].priority)
    {
        swapHeapNodes(&fringe->heap[index], &fringe->heap[parent]);
        perculateUp(fringe, parent);
    }
    
}

long int dequeue(struct Heap *fringe, char *pos){
    /*Assumes heap is not empty*/
    long int parentIndex = fringe->heap[0].parentIndex;
    strcpy(pos, fringe->heap[0].pos);
    swapHeapNodes(&fringe->heap[0], &fringe->heap[fringe->tail]);
    fringe->tail--;
    perculateDown(fringe, 0);
    return parentIndex;
}

void enqueue(struct Heap *fringe, char *pos, long int priority, long int parentIndex){
    fringe->tail++;
    strcpy(fringe->heap[fringe->tail].pos, pos);
    free(pos);
    fringe->heap[fringe->tail].priority=priority;
    fringe->heap[fringe->tail].parentIndex=parentIndex;
    perculateUp(fringe, fringe->tail);
}

void printHeap(struct Heap *fringe){
    printf("Heap w/%ld: \n", fringe->tail+1);
    for(int i=0; i<=fringe->tail;i++){
        printPosition(fringe->heap[i].pos);
    }
    printf("Heap End\n");
}

unsigned long hash(unsigned char *str){
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

int insert(struct HashTable * explored, char *pos, long int parentIndex){
    unsigned long h = hash(pos) % HASHTABLE_SIZE;
    int i, j = 0;
    for(; j < HASHTABLE_SIZE/2 + 2; j++){
        i = (h + j * j) % HASHTABLE_SIZE;
        if( explored->arr[i][0] == '\0' ){
            explored->count++;
            strcpy(explored->arr[i], pos);
            explored->parent[i]=parentIndex;
            return i;
        }
    } 
}

int contains(struct HashTable *explored, char *pos){
    unsigned long h = hash(pos) % HASHTABLE_SIZE;
    int i, j = 0;
    for(; j < HASHTABLE_SIZE/2 + 2; j++){
        i = (h + j * j) % HASHTABLE_SIZE;
        if (strcmp(explored->arr[i], pos) == 0) return 1;
        if( explored->arr[i][0] == '\0' ) return 0; 
    }
    return 0;
}

char *newPos(char *pos, int old, int new){
    char *newPos=malloc(sizeof(char)*10);
    strcpy(newPos,pos);
    char temp=newPos[old];
    newPos[old]=newPos[new];
    newPos[new]=temp;
    return newPos;
}


int indexOfNum(char *pos, char num){
    int i;
    for(i=0;i<9;i++)
        if(num==*(pos+i)) 
            return i;
    return -1;
}

char manhattanDistance(char *pos, char *goal){ 
    int sum = 0;
    for(char i = '1'; i < '9'; i++){
        int curr = indexOfNum(pos,i);
        int dest = indexOfNum(goal,i);
        sum += abs( curr % 3 - dest % 3 ) + abs(curr / 3 - dest / 3);
    }

    return sum;
}

char updateManhattanDistance(char*goal, char movingNum , int newSpot, int oldSpot){
    int dest = indexOfNum(goal, movingNum);
    return abs( newSpot % 3 - dest % 3 ) + abs(newSpot / 3 - dest / 3)  - abs( oldSpot % 3 - dest % 3 ) - abs(oldSpot / 3 - dest / 3);
}

int isValidFormat(char *pos)
{
    int i;
    int check[9];
    int number;
    for(i=0;i<9;i++)
        check[i]=0;
    

    if(strlen(pos)!=9)
    {
        printf("Starting position \'%s\' is not 9 digits long\n",pos);
        return 0;
    }
    
    
    for(i=0;i<9;i++)
    {
        char digit[2];
        digit[0]=pos[i];
        switch (digit[0])
        {
        case '0':case '1': case '2': case '3': case '4': case '5':case '6': case '7': case '8':  
            break;
        
        default:
            printf("Invalid digit: %c\n",digit[0]);
            return 0;
        }

        digit[1]='\0';
        number=atoi(digit);
        
        if(!check[number]) 
            check[number]++;
        else
        {
            printf("Duplicate digit: %d\n",number);
            return 0;
        }

    }

    return 1;
}

void printPath(struct HashTable *explored, long int currIndex){
    int count = -1;
    printf("Solution: \n");
    for(;currIndex!=-1;currIndex=explored->parent[currIndex], count++)
        printPosition(explored->arr[currIndex]);
    printf("Solution Steps: %d\n",count);
}

int search(char *init, char *goal){

    struct Heap fringe;
    fringe.tail=0;
    strcpy(fringe.heap[0].pos, init);
    fringe.heap[0].priority=manhattanDistance(init, goal);
    fringe.heap[0].parentIndex=-1;

    
    struct HashTable explored;
    explored.count=0;
    for(long int i = 0;i<HASHTABLE_SIZE;i++){
        explored.arr[i][0]='\0';
        explored.parent[i]=-1;
    }

    char currPos[10];
    char currManDist;
    long int parentIndex;
    long int currIndex;
    while(fringe.tail > -1){
        //printf("Explored size: %ld\n",explored.count);
        //printHeap(&fringe);

        currManDist = fringe.heap[0].priority;
        currManDist++;
        parentIndex = dequeue(&fringe, currPos);
        /*
        printf("Looking at\n");
        printPosition(currPos);
        printf("---\n");
        */
        if (!contains(&explored, currPos)){
            
            currIndex = insert(&explored,currPos, parentIndex);
            
            if ( strcmp(currPos, goal) == 0 ){
                printPath(&explored,currIndex);
                printf("Combinations Searched: %ld \n", explored.count);
                return 1;
            }

            switch (indexOfNum(currPos, '0'))
            {
            case 0:
                enqueue(&fringe, newPos(currPos,0,1), currManDist + updateManhattanDistance(goal, currPos[1], 0, 1), currIndex);
                enqueue(&fringe, newPos(currPos,0,3), currManDist + updateManhattanDistance(goal, currPos[3], 0, 3), currIndex);
                break;
            case 1:
                enqueue(&fringe, newPos(currPos,1,0), currManDist + updateManhattanDistance(goal, currPos[0], 1, 0), currIndex);
                enqueue(&fringe, newPos(currPos,1,2), currManDist + updateManhattanDistance(goal, currPos[2], 1, 2), currIndex);
                enqueue(&fringe, newPos(currPos,1,4), currManDist + updateManhattanDistance(goal, currPos[4], 1, 4), currIndex);
                break;
            case 2:
                enqueue(&fringe, newPos(currPos,2,1), currManDist + updateManhattanDistance(goal, currPos[1], 2, 1), currIndex);
                enqueue(&fringe, newPos(currPos,2,5), currManDist + updateManhattanDistance(goal, currPos[5], 2, 5), currIndex);
                break;
            case 3:
                enqueue(&fringe, newPos(currPos,3,0), currManDist + updateManhattanDistance(goal, currPos[0], 3, 0), currIndex);
                enqueue(&fringe, newPos(currPos,3,4), currManDist + updateManhattanDistance(goal, currPos[4], 3, 4), currIndex);
                enqueue(&fringe, newPos(currPos,3,6), currManDist + updateManhattanDistance(goal, currPos[6], 3, 6), currIndex);
                break;
            case 4:
                enqueue(&fringe, newPos(currPos,4,1), currManDist + updateManhattanDistance(goal, currPos[1], 4, 1), currIndex);
                enqueue(&fringe, newPos(currPos,4,3), currManDist + updateManhattanDistance(goal, currPos[3], 4, 3), currIndex);
                enqueue(&fringe, newPos(currPos,4,5), currManDist + updateManhattanDistance(goal, currPos[5], 4, 5), currIndex);
                enqueue(&fringe, newPos(currPos,4,7), currManDist + updateManhattanDistance(goal, currPos[7], 4, 7), currIndex);
                break;
            case 5:
                enqueue(&fringe, newPos(currPos,5,2), currManDist + updateManhattanDistance(goal, currPos[2], 5, 2), currIndex);
                enqueue(&fringe, newPos(currPos,5,4), currManDist + updateManhattanDistance(goal, currPos[4], 5, 4), currIndex);
                enqueue(&fringe, newPos(currPos,5,8), currManDist + updateManhattanDistance(goal, currPos[8], 5, 8), currIndex);
                break;
            case 6:
                enqueue(&fringe, newPos(currPos,6,3), currManDist + updateManhattanDistance(goal, currPos[3], 6, 3), currIndex);
                enqueue(&fringe, newPos(currPos,6,7), currManDist + updateManhattanDistance(goal, currPos[7], 6, 7), currIndex);
                break;
            case 7:
                enqueue(&fringe, newPos(currPos,7,4), currManDist + updateManhattanDistance(goal, currPos[4], 7, 4), currIndex);
                enqueue(&fringe, newPos(currPos,7,6), currManDist + updateManhattanDistance(goal, currPos[6], 7, 6), currIndex);
                enqueue(&fringe, newPos(currPos,7,8), currManDist + updateManhattanDistance(goal, currPos[8], 7, 8), currIndex);
                break;
            case 8:   
                enqueue(&fringe, newPos(currPos,8,5), currManDist + updateManhattanDistance(goal, currPos[5], 8, 5), currIndex);
                enqueue(&fringe, newPos(currPos,8,7), currManDist + updateManhattanDistance(goal, currPos[7], 8, 7), currIndex);
                break;
            default:
                break;
            }


        }

    }

    printf("Explored Size: %ld\n", explored.count);

    return 0;
}

int indexOf(int *nums, int num){
    int i = 0;
    while(num != nums[i]) i++;
    return i;
}

int isSolvable(char *init, char *goal){
   int nums[8];
    for(int i = 0; i < 9; i++){
        nums[i] = goal[i]-'0';
    }

    int numInversions=0;
    for(int i = 0; i < 9; i++){
        for(int j = i + 1; j < 9; j++){
            if( (init[i] != '0') && (init[j] != '0') &&  indexOf(nums, init[i] - '0') < indexOf( nums, init[j] - '0'))
                numInversions++;
        }
    }

    return numInversions % 2 == 0;
}

int main(int argc, char *argv[]){
    clock_t t = clock();
    
    if(argc == 1){
		printf("Missing starting position\n");
		return -1;
    }
	if(argc == 2){
        printf("Missing ending position\n");
        return -1;
    }
    if(argc > 3){
        printf("Too many arguments\n");
        return -1;
    }
    if(!isValidFormat(argv[1])){
        printf("Not a valid starting position\n");
        return -1;
    }
    if(!isValidFormat(argv[2])){
        printf("Not a valid ending position\n");
        return -1;
    }  
    
    if(!isSolvable(argv[1], argv[2])){
        printf("This puzzle cannot be solved\n");
        return -1;
    }

    if(!search(argv[1], argv[2]))
        printf("No Answer Found\n");

    t = clock()-t;
    printf("Time taken: %f seconds\n",((double)t)/CLOCKS_PER_SEC);

    return 0;
}
