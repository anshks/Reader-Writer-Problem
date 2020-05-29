#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include <time.h>
#define BUFSIZE  256

struct element {
    int name;
    struct element *next;
};
int size = 0;
struct element *tail;

void init_queue (void)
{
    tail = NULL;
}

void enqueue (int name)
{
    struct element *ptr;
    int data;
    ptr = (struct element *) malloc (sizeof (struct element));


    data = name;
    ptr -> name = data;

    if (tail == NULL) {
        ptr -> next = ptr;
    }
    else
    {
        ptr -> next = tail -> next;
        tail -> next = ptr;
    }
    tail = ptr;
    size++;
}

int dequeue ()
{
    struct element *ptr;
    int cp;

    if (!tail) {
        fprintf (stderr, "Queue is empty\n");
        return -1;
    }
    size--;

    ptr = tail -> next;
    cp = ptr -> name;

    if (ptr == tail)
        tail = NULL;
    else
        tail -> next = ptr -> next;
    free (ptr);
    return cp;
}
int print_first(){
    int cp;

    if (!tail || !(tail -> next)) {
        fprintf (stderr, "Queue is empty\n");
        return -1;
    }
    cp = tail  -> name;
    return cp;
}
int print_last(int id){
    struct element *ptr, *head;

    if (!tail || !(tail -> next)) {
        fprintf (stderr, "Queue is empty\n");
        return -1;
    }

    printf("\n");

    head = ptr = tail -> next;
    int i = 1;
    double waittime = 0.05;
    do {
        printf("\nReader_head - > %d := index -> %d := data ->  %d",id,i,ptr -> name);
        ptr = ptr -> next;
        usleep(10*1000);
        i++;
    } while (ptr -> next != head);
    return ptr -> name;
}

pthread_mutex_t x,x1,wsem,tmp;
pthread_t tid;
int readcount,readcount1;

void intialize()
{
    pthread_mutex_init(&x,NULL);
    pthread_mutex_init(&x1,NULL);
    pthread_mutex_init(&wsem,NULL);
    pthread_mutex_init(&tmp,NULL);
    readcount=0;
    readcount1= 0;
}

void * reader_head(void* param)
{   
    int id = (int) param;
    int waittime;
    waittime = rand() % 5;
    printf("\nReader_head %d is trying to enter", id+1);
        printf("\nReader_head %d is inside",id+1);
    print_last(id + 1);

    pthread_mutex_lock(&tmp);
    
    // pthread_mutex_lock(&x1);
        
    readcount1++;

    if(readcount1==1)
        pthread_mutex_lock(&wsem);
    // pthread_mutex_unlock(&x1);
    printf("\nReader_head %d is inside last",id+1);
    // sleep(waittime);
    // pthread_mutex_lock(&x1);
    print_last(id + 1);
    int data = print_first();
    if(data != -1)
        printf("\nReader_head - > %d := index -> %d := data ->  %d",id + 1,size,data);
     readcount1--;
    if(readcount1==0)
        pthread_mutex_unlock(&wsem);
    // pthread_mutex_unlock(&x1);
    pthread_mutex_unlock(&tmp);
    printf("\nReader_head %d is  outside",id+1);
    printf("\nReader_head %d is Leaving", id+1);
}   
void * reader(void* param)
{   
    int id = (int) param;
    int waittime;
    waittime = rand() % 5;
    printf("\nReader %d is trying to enter", id+1);
    pthread_mutex_lock(&x);
    readcount++;
    if(readcount==1)
        pthread_mutex_lock(&wsem);
    printf("\nReader %d is inside ",id+1);
    pthread_mutex_unlock(&x);
    // printf("\nReader %d time : %d",id+1,waittime);
    sleep(waittime);
    pthread_mutex_lock(&x);
    int data = print_first();
    if(data != -1)
        printf("\nReader %d read : %d ",id+1,data);
    readcount--;
    if(readcount==0)
        pthread_mutex_unlock(&wsem);
    pthread_mutex_unlock(&x);
    printf("\nReader %d is  outside",id+1);
    printf("\nReader %d is Leaving", id+1);
}   
void * writer (void * param)
{   

    int id = (int) param;
    int waittime;
    waittime=rand() % 3;
    printf("\nWriter %d is trying to enter",id+1);
    pthread_mutex_lock(&wsem);
    printf("\nWriter %d has entered",id+1);

    sleep(waittime);
    pthread_mutex_lock(&tmp);
    int data = random_int(100);
    enqueue(data);
    pthread_mutex_unlock(&tmp);
    printf("\nWriter %d write : %d ",id+1,data);
    pthread_mutex_unlock(&wsem);    
    printf("\nWriter %d is leaving",id+1);
}
void * read_dequeue (void * param){
    int id = (int) param;
    int waittime;
    waittime=rand() % 3;
    printf("\nread_deque %d is trying to enter",id+1);
    pthread_mutex_lock(&wsem);
    printf("\nread_dequeue %d has entered",id+1);

    sleep(waittime);
    int data = dequeue();
    if(data != -1)
        printf("\nread_deque %d read : %d ",id+1,data);
    pthread_mutex_unlock(&wsem);    
    printf("\nread_deque %d is leaving",id+1);
}

int random_int(int k){
    return rand()%k;
}
int clean_stdin()
{
    while (getchar()!='\n');
    return 1;
}

int main(){
    intialize();
    init_queue();
    int n2;
    char c;
    do
    {  
        printf("\nEnter number of writers: \n");

    } while (((scanf("%d%c", &n2, &c)!=2 || c!='\n') && clean_stdin()) || n2 <= 0);
    
    for (int i = 0; i < n2; ++i)
    {
        pthread_create(&tid,NULL,writer,(void *) i);
    }
    // sleep(10);
    for (int i = 0; i < n2; ++i)
    {
        pthread_create(&tid,NULL,writer,(void *) i);
        // pthread_create(&tid,NULL,reader,(void *) i);
        pthread_create(&tid,NULL,reader_head,(void *) i);
        pthread_create(&tid,NULL,read_dequeue,(void *) i);
        // pthread_create(&tid,NULL,writer,(void *) i);
    }
    sleep(50);
}