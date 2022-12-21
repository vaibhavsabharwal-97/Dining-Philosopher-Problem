#include<stdio.h>
#include<semaphore.h>
#include<pthread.h>
#include<unistd.h>
#define N 5                		 
#define THINKING 0           
#define HUNGRY 1
#define EATING 2
#define LEFT (ph_num+4)%N   
#define RIGHT (ph_num+1)%N
sem_t mutex;
sem_t S[N];
int count[5];
int FOOD = 0;
void * philospher(void *num);
void take_fork(int);
void put_fork(int);
void test(int);
int state[N];                	                
int phil_num[N]={0,1,2,3,4};      
int main()                      	  
{
    int i;
    pthread_t thread_id[N];       
    sem_init(&mutex,0,1);       
    for(i=0;i<N;i++)
        sem_init(&S[i],0,0);
    for(i=0;i<N;i++)                 
    {
        pthread_create(&thread_id[i],NULL,philospher,&phil_num[i]);
    }
    for(i=0;i<N;i++)
        pthread_join(thread_id[i],NULL);             
    for(i=0;i<N;i++)
    printf("Philospher %d ate %d \n",i,count[i]);   
					 
    printf("\n");
}
void *philospher(void *num)
{   
    while(FOOD <= 20)         
    {
        int *i = num;               
        usleep(10000);
        take_fork(*i);
        put_fork(*i);
    }
}
void take_fork(int ph_num)       
{
    sem_wait(&mutex);               
    state[ph_num] = HUNGRY;
    test(ph_num);
    sem_post(&mutex);
    sem_wait(&S[ph_num]);
    usleep(10000);
}
void test(int ph_num)
{
    if (state[ph_num] == HUNGRY && state[LEFT] != EATING && state[RIGHT] 
!= EATING)
    {                                    		     
        state[ph_num] = EATING;
        usleep(20000);
        sem_post(&S[ph_num]);
    }
}
void put_fork(int ph_num)              
{
    sem_wait(&mutex);
    state[ph_num] = THINKING;

    count[ph_num]++;
    FOOD++;
  
    test(LEFT);
    test(RIGHT);

    printf("#Eating Count = %d \n", FOOD);
    int i;
    for(i=0;i<5;i++){                   
        if(state[i]==EATING)      
            printf("Philosopher %d is eating\n", i);
        else if(state[i]==HUNGRY)
            printf("Philosopher %d is waiting and calling pickup()\n", i);
        else if(state[i]==THINKING)
            printf("Philosopher %d is thinking\n", i);
    }

    sem_post(&mutex);
}
