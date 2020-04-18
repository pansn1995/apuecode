#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
 
/*链表作为共享数据，需被互斥量保护*/
struct msg {
    struct msg *next;
    int num;
};
 
struct msg *head;
struct msg *mp;
 
/*静态初始化一个全局条件变量和一个全局互斥量*/
pthread_cond_t has_product = PTHREAD_COND_INITIALIZER;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
 
void *consumer(void *p)
{
    for(;;){
        pthread_mutex_lock(&lock);
        while(head == NULL){ //头指针为空，说明没有节点。使用while而不使用if。
            pthread_cond_wait(&has_product, &lock);
//注意，在wait阻塞期间，该线程会释放之前lock的锁，因此生产者是可以加锁的。每当有signal或者broadcast has_product时，该函数便返回，while再次判断head是否为NULL，条件为否，因此可以向下执行。如果这里是if而不是while的话，那么直接往下执行。而signal的发送是保证至少有一个线程是捕获到，因此如果有两个线程同时捕捉到，并且wait返回时，虽然一个线程是会锁定互斥变量，成功操作，但是另一个线程由于没有再次判断head是否为NULL，因此等他获得锁再操作时，其实head已经为NULL了（上一个线程消费了唯一的一个产品），因此发生段错误。因此这里必须要使用while，从而可以再次wait检查锁占用情况。
        }
        mp = head;
        head = mp->next; //模拟消费掉一个产品
        printf("-Consume ---%d\n", mp->num);
        free(mp); 
		mp = NULL;
	pthread_mutex_unlock(&lock);
 
        //sleep(rand() % 5);
    }
}
 
void *producer(void *p)
{
for(;;){
 
		pthread_mutex_lock(&lock);//防止mp刚malloc出来就又被新malloc出来的替代。
        mp = malloc(sizeof(struct msg));
        //模拟生产一个产品
        mp->num = rand() % 1000 + 1;
        printf("-Produce ---%d\n", mp->num);
 
        mp->next = head;
        head = mp;
        pthread_mutex_unlock(&lock);
        //将等待在该条件变量上的一个线程唤醒
        pthread_cond_signal(&has_product);
		
        //sleep(rand() % 5);
    }
}
 
int main(int argc, char * argv)
{
    pthread_t pid, cid,pid2;
    srand(time(NULL));
 
    pthread_create(&pid, NULL, producer, NULL);
	 pthread_create(&pid2, NULL, consumer, NULL);
    pthread_create(&cid, NULL, consumer, NULL);
 
    pthread_join(pid, NULL);
	pthread_join(pid2, NULL);
    pthread_join(cid, NULL);
 
    return 0;
}
