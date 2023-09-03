#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>


struct Node {
    char *pos;
    int index;
    int parent;
    struct Node *next;
};

struct List {
    struct Node *head;
    struct Node *tail;
};

struct List newSLL() {
    /* construct an empty list */
    struct List newList;
    newList.head=NULL;
    newList.tail=NULL;
    return newList;
}

int contains(struct List *list, char *pos)
{
    
    struct Node *p;
    for(p=list->head;p!=NULL;p=p->next)
    {
        //printf("Comparing %s to %s begets %d\n",p->pos,pos,strcmp(p->pos,pos));
        if(strcmp(p->pos,pos)==0)
        {
            //printf("Does cointain %s\n",pos);
            return 1;
        }
    }
    //printf("NO contains\n");
    return 0;
}


int pop(struct List *list, char *pos)
{
    struct Node *p=list->head;
    char *temp=p->pos;
    int parentIndex=p->parent;
    strcpy(pos,p->pos);
    list->head=p->next;
    if(list->head==NULL)
        list->tail=NULL;
    free(p);
    return parentIndex;

}

int listLength(struct List *list){
    struct Node *p;
    int count=0;
    for(p=list->head;p!=NULL;p=p->next)
        count++;
    return count;
}

void push(struct List *list, char *pos, int parentIndex,int currIndex){
    struct Node *p=malloc(sizeof(struct Node));
    p->pos=malloc(sizeof(char)*9);
    p->next=list->head;
    p->parent=parentIndex;
    p->index=currIndex;
    strcpy(p->pos,pos);
    if(list->head==NULL)
        list->tail=p;
    list->head=p;
}


//Add index init
void append(struct List *list, char *pos, int parentIndex,int currIndex){
    //printf("In append: %s\n",pos);
    if(list->head==NULL)
        push(list,pos,parentIndex,currIndex);
    else{
        struct Node *p=malloc(sizeof(struct Node));
        p->pos=malloc(sizeof(char)*9);
        p->next=NULL;
        p->index=currIndex;
        p->parent=parentIndex;
        strcpy(p->pos,pos);
        list->tail->next=p;
        list->tail=p;
    }
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

int indexOfZero(char *pos)
{
    int i;
    for(i=0;i<9;i++)
        if('0'==*(pos+i)) 
            return i;
    return -1;
}

char *newPos(char *pos,int old,int new)
{
    char *newPos=malloc(sizeof(char)*9);
    strcpy(newPos,pos);
    char temp=*(newPos+old);
    *(newPos+old)=*(newPos+new);
    *(newPos+new)=temp;

    return newPos;
}

void printList(struct List *list)
{
    struct Node *p;
    int count=0;
    for(p=list->head;p!=NULL;p=p->next){
        printPosition(p->pos);
        count++;
    }
    printf("List Length: %d\n",count);
            
}

struct Node *getParent(struct List *list, int parentIndex)
{
    struct Node *p;
    for(p=list->head;p!=NULL;p=p->next)
    {
        if(p->index==parentIndex)
        {
            return p;
        }
    }
    printf("Parent not found\n");
    return NULL;
}

void printPath(struct List *list, int start, char *goal)
{
    int count=0;
    printf("Solution: \n");
    printPosition(goal);
    struct Node *p;
    for(p=getParent(list,start);p!=NULL;p=getParent(list,p->parent)){
        printPosition(p->pos);
        count++;
    }
    printf("Solution steps: %d\n",count);

}

int search(char *init, char *goal)
{
    int count=0;
    int parentIndex;
    char currPos[10];
    struct List fringe= newSLL();
    push(&fringe,init,-1,count);
    struct List explored= newSLL();
    
    while(fringe.head!=NULL)
    {
        parentIndex=pop(&fringe,currPos);
        /*
        printf("Current Pos: \n");
        printPosition(currPos);
        printf("Explored: \n");
        printList(&explored);
        */
        if(count%10000==0)printf("Explored size: %d\n",count);
        if(!contains(&explored,currPos))
        {
            if(strcmp(currPos,goal)==0) 
            {
                printPath(&explored,parentIndex,goal);
                printf("Explored size: %d\n",count);
                return 1;
            }
    

            /*Here goes the succesors*/
           
            //printf("%d\n",indexOfZero(currPos));
            switch (indexOfZero(currPos))
            {
            case 0:
                append(&fringe,newPos(currPos,0,1),count,count);
                append(&fringe,newPos(currPos,0,3),count,count);
                break;
            case 1:
                append(&fringe,newPos(currPos,1,0),count,count);
                append(&fringe,newPos(currPos,1,2),count,count);
                append(&fringe,newPos(currPos,1,4),count,count);
                break;
            case 2:
                append(&fringe,newPos(currPos,2,1),count,count);
                append(&fringe,newPos(currPos,2,5),count,count);
                break;
            case 3:
                append(&fringe,newPos(currPos,3,0),count,count);
                append(&fringe,newPos(currPos,3,4),count,count);
                append(&fringe,newPos(currPos,3,6),count,count);
                break;
            case 4:
                append(&fringe,newPos(currPos,4,1),count,count);
                append(&fringe,newPos(currPos,4,3),count,count);
                append(&fringe,newPos(currPos,4,5),count,count);
                append(&fringe,newPos(currPos,4,7),count,count);
                break;
            case 5:
                append(&fringe,newPos(currPos,5,2),count,count);
                append(&fringe,newPos(currPos,5,4),count,count);
                append(&fringe,newPos(currPos,5,8),count,count);
                break;
            case 6:
                append(&fringe,newPos(currPos,6,3),count,count);
                append(&fringe,newPos(currPos,6,7),count,count);
                break;
            case 7:
                append(&fringe,newPos(currPos,7,4),count,count);
                append(&fringe,newPos(currPos,7,6),count,count);
                append(&fringe,newPos(currPos,7,8),count,count);
                break;
            case 8:
                append(&fringe,newPos(currPos,8,5),count,count);
                append(&fringe,newPos(currPos,8,7),count,count);
                break;
            default:
                break;
            }

            
            push(&explored,currPos,parentIndex,count);
            count++;
        }
    }
    return 0;
}

int main(int argc, char *argv[])
{
    clock_t t=clock();
    if(argc == 1 ){
		printf("Missing starting position\n");
		return -1;
	}else if(argc == 2){
        printf("Missing ending position\n");
        return -1;
    }else if(argc > 3){
        printf("Too many arguments\n");
        return -1;
    }else if(!isValidFormat(argv[1])){
        printf("Not a valid starting position\n");
        return -1;
    }else if(!isValidFormat(argv[2])){
        printf("Not a valid ending position\n");
        return -1;
    }  

    //printPosition(argv[1]);
    //printPosition(newPos(argv[1],0,1));
    
    if(search(argv[1],argv[2]))
        printf("Answer Found!\n");
    else 
        printf("Bruh moment\n");
    
    t=clock()-t;
    printf("Time taken: %f seconds\n",((double)t)/CLOCKS_PER_SEC);

    return 0;
}
