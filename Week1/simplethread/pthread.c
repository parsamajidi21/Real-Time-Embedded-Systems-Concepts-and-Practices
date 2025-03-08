#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <sched.h>
#include <sys/utsname.h>
#include <syslog.h>

#define NUM_THREADS 12

typedef struct
{
    int threadIdx;
} threadParams_t;

struct utsname unameData;

// POSIX thread declarations and scheduling attributes
//
pthread_t threads[NUM_THREADS];
threadParams_t threadParams[NUM_THREADS];


void *counterThread(void *threadp)
{
    int sum=0, i;
    threadParams_t *threadParams = (threadParams_t *)threadp;

    for(i=1; i < (threadParams->threadIdx)+1; i++)
        sum=sum+i;
 
    printf("Thread idx=%d, sum[0...%d]=%d\n", 
           threadParams->threadIdx,
           threadParams->threadIdx, sum);
    if (threadParams->threadIdx == NUM_THREADS - 1)
	    syslog(LOG_INFO, "[COURSE:1][ASSIGNMENT:1]: Hello World from Thread!");
}

void _uname() {
    char buffer[1024];
    system("sudo truncate -s 0 /var/log/syslog");
    FILE *uname_cmd = popen("uname -a", "r");
    if (uname_cmd == NULL)
        perror("Command failed");
    fgets(buffer, sizeof(buffer), uname_cmd);
    pclose(uname_cmd);

    openlog("pthread", LOG_PID|LOG_CONS, LOG_USER);
    syslog(LOG_INFO, "[COURSE:1][ASSIGNMENT:1] %s", buffer);
    closelog();
}

int main (int argc, char *argv[])
{
   int rc;
   int i;

   _uname();
   syslog(LOG_INFO,"[COURSE:1][ASSIGNMENT:1]: Hello World from Main!\n");

   for(i=0; i < NUM_THREADS; i++)
   {
       threadParams[i].threadIdx=i;

       pthread_create(&threads[i],   // pointer to thread descriptor
                      (void *)0,     // use default attributes
                      counterThread, // thread function entry point
                      (void *)&(threadParams[i]) // parameters to pass in
                     );

   }
   for(i=0;i<NUM_THREADS;i++)
       pthread_join(threads[i], NULL);

   printf("TEST COMPLETE\n");
}
