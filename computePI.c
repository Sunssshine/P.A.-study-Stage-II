#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <sys/types.h>
#include <sys/time.h>

#define TRUE_PI 3.14159265358979
#define DEFAULT_ACCURACY_LIMIT 13371337
#define DEFAULT_THREADS_NUMBER 4
#define ONE_THREAD 1

long double calcPI(int, int);

int main(int argc, char ** argv)
{
    int threadCount;
    int accuracyLimit;
    
    printf("\n------------\n"
           "Initializing\n"
           "------------\n\n");

    switch (argc)
    {
        case 1:
            threadCount = DEFAULT_THREADS_NUMBER;
            accuracyLimit = DEFAULT_ACCURACY_LIMIT;
            printf("Whoa u didn`t say number of threads and even accuracy limit to me, well, ok...\n"
                   "* Number of threads is: %d\n"
                   "* Accuracy limit is: %d\n"
                   "- SHAME ON YOU -\n",
                    DEFAULT_THREADS_NUMBER, DEFAULT_ACCURACY_LIMIT);
                    
            break;        
        
        case 2:
            threadCount = atoi(argv[1]);
            accuracyLimit = DEFAULT_ACCURACY_LIMIT;
            printf("Whoa u didn`t say accuracy limit to me, well, ok, it's %d now. ._.\n", DEFAULT_ACCURACY_LIMIT);
            break;

        case 3:
            threadCount = atoi(argv[1]);
            accuracyLimit = atoi(argv[2]);

            printf("* Number of threads is: %d\n"
                   "* Accuracy limit is: %d\n",
                    threadCount, accuracyLimit);
            break;

        default:
            threadCount = atoi(argv[1]);
            accuracyLimit = atoi(argv[2]);

            printf("Whoa, I got too much arguments so...\nI will just ignore all except first two, alright? #_#\n"
                   "* Number of threads is: %d\n"
                   "* Accuracy limit is: %d\n",
                    threadCount, accuracyLimit);
            break;
    }

    if(threadCount > accuracyLimit)
        threadCount = accuracyLimit;

    long double result;
    long startTime;
    long finishTime;
    long resultTime;
    struct timeval  tv;


    // ONE THREAD BLOCK

    gettimeofday(&tv, NULL);
    startTime = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000;
    result = calcPI(1, accuracyLimit);

    if(result < 0)
    {
        exit(0);
    }

    gettimeofday(&tv, NULL);
    finishTime = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000;

    resultTime = finishTime - startTime;
    printf("Calculated PI: %.14Lf\nTime for %d threads and %d accuracy: %ld ms\n",result, ONE_THREAD, accuracyLimit, resultTime);

    // USER THREAD BLOCK

    gettimeofday(&tv, NULL);
    startTime = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000;

    result = calcPI(threadCount, accuracyLimit);
    if(result < 0)
    {
        exit(0);
    }

    gettimeofday(&tv, NULL);
    finishTime = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000;

    resultTime = finishTime - startTime;
    printf("Calculated PI: %.14Lf\nTime for %d threads and %d accuracy: %ld ms",result, threadCount, accuracyLimit, resultTime);

    printf("\n\nResult PI is: %.14Lf", result);
    printf("\nTrue PI value is: %.14f", TRUE_PI);
    printf("\nDiff~ between my and true PI is: |%.14Lf|\n\n----------------------<END>-------------------------\n", result-TRUE_PI);

    return 0;
}

long double calcPI(int threadQuantity, int accuracyLimit)
{
    printf("\n ...START COMPUTING... \n");
    pid_t childPid;

    int limitForThread = accuracyLimit/threadQuantity;       
    int alignment = accuracyLimit % threadQuantity;
    long double calculatedPI = 0;  

    int i;

    int ** fds = (int**)malloc(sizeof(int*)*threadQuantity);
    for(i = 0; i<threadQuantity; i++)
    {
        fds[i] = (int*)malloc(sizeof(int)*2);
        if(pipe(fds[i])<0)
        {
            printf("\nPipe error\n");
            return -1;
        }
    }

    for(i = 0; i<threadQuantity; i++)
    {

       childPid = fork();
       if(!childPid)
       {
            int j;
            long double result = 0;
            //printf("\nTHREAD #%d\n", i);
            if((i==threadQuantity-1) && (alignment != 0))
            {
                for(j = limitForThread*i; j < (limitForThread * (i+1)) + alignment; j++)
                {
                    //printf("%d ", j);
                    result += 4*((powl(-1, j))/(2*j+1));
                }
            }
            else
            {
                for(j = limitForThread*i; j < limitForThread * (i+1); j++)
                {
                    //printf("%d ", j);
                    result += 4*((powl(-1, j))/(2*j+1));
                }
                
            }

            //printf("\n--------\n%.20Lf\n", result);
            close(fds[i][0]);
            write(fds[i][1], &result, sizeof(long double));
            return -1;
       }
       else if(childPid > 0)
       {
           //wait();
           // logic for parent
       }
       else
       {
           // logic for error
           printf("Thread error");
           return -1;
       } 
    }

    for(i = 0; i<threadQuantity; i++)
        wait(0);

    for(i = 0; i<threadQuantity; i++)
    {
        close(fds[i][1]);
        long double threadRes;
        read(fds[i][0], &threadRes, sizeof(long double));
        calculatedPI+=threadRes;
        //printf("%Lf ", threadRes);
    }
    return calculatedPI;
}