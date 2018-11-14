#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#define DEFAULT_ACCURACY_LIMIT 100
#define DEFAULT_THREADS_NUMBER 4

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
            printf("Whoa u didn`t say number of threads and even accuracy limit to me, well, ok...\n"
                   "* Number of threads is: %d\n"
                   "* Accuracy limit is: %d\n"
                   "- SHAME ON YOU -\n\n",
                    DEFAULT_THREADS_NUMBER, DEFAULT_ACCURACY_LIMIT);
            break;        
        
        case 2:
            printf("Whoa u didn`t say accuracy limit to me, well, ok, it's %d now. ._.\n\n", DEFAULT_ACCURACY_LIMIT);
            break;

        case 3:
            threadCount = atoi(argv[1]);
            accuracyLimit = atoi(argv[2]);

            printf("* Number of threads is: %d\n"
                   "* Accuracy limit is: %d\n\n",
                    threadCount, accuracyLimit);
            break;

        default:
            threadCount = atoi(argv[1]);
            accuracyLimit = atoi(argv[2]);

            printf("Whoa, I got too much arguments so...\nI will just ignore all except first two, alright? #_#\n"
                   "* Number of threads is: %d\n"
                   "* Accuracy limit is: %d\n\n",
                    threadCount, accuracyLimit);
            break;
    }

    return 0;
}