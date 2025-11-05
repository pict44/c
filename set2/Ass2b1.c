/*
Name:-Ayush Karanjkhele
Roll No:-I3104
ASSIGNMENT 2(B):-

Problem Statement:-
 Process Control system calls. The demonstratiion of FORK,EXECVE and WAIT system calls along with zombieand orphan states.

b. Implement the C program in which main program accepts an integer array. Main program uses the FORK system call to create a new process called a child process. Parent process sorts an integer array and passes the sorted array to child process through the command line arguments of EXECVE system call. The child process uses EXECVE system call to load new program that uses this sorted array for performing the binary search to search the particular item in the array.
*/
// parent.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

// ascending bubble sort
void sortAscending(int arr[], int n) {
    int i, j, tmp;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <child_program_path> <numbers...>\n", argv[0]);
        return 1;
    }

    int n = argc - 2;
    int arr[n];
    for (int i = 0; i < n; i++) {
        arr[i] = atoi(argv[i + 2]);
    }

    printf("[Parent] Original array:\n");
    for (int i = 0; i < n; i++) printf("%d ", arr[i]);
    printf("\n");

    sortAscending(arr, n);

    printf("[Parent] Sorted ascending:\n");
    for (int i = 0; i < n; i++) printf("%d ", arr[i]);
    printf("\n");

    // prepare args for child
    char *childArgs[n + 2];
    childArgs[0] = argv[1]; // child program name
    for (int i = 0; i < n; i++) {
        char *num = malloc(10);
        sprintf(num, "%d", arr[i]);
        childArgs[i + 1] = num;
    }
    childArgs[n + 1] = NULL;

    pid_t pid = fork();

    if (pid == 0) {
        printf("[Child forked] PID=%d, calling execv...\n", getpid());
        execv(argv[1], childArgs);
        perror("execv failed");
        exit(1);
    } else {
        wait(NULL);
        printf("[Parent] Child finished.\n");
    }

    return 0;
}
/*
Second code-
// child.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// descending bubble sort
void sortDescending(int arr[], int n) {
    int i, j, tmp;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (arr[j] < arr[j + 1]) {
                tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
        }
    }
}

int main(int argc, char *argv[]) {
    int n = argc - 1;
    int arr[n];
    for (int i = 0; i < n; i++) {
        arr[i] = atoi(argv[i + 1]);
    }

    printf("[Running Second code in child's image.See they have same PID] PID=%d\n", getpid());

    printf("[Child] Received (ascending):\n");
    for (int i = 0; i < n; i++) printf("%d ", arr[i]);
    printf("\n");

    sortDescending(arr, n);

    printf("[Child] Sorted descending:\n");
    for (int i = 0; i < n; i++) printf("%d ", arr[i]);
    printf("\n");

    return 0;
}

*/
// Out put
/*
ayush@Legion:~/Ayush_I3104$ gcc Ass2b1.c -o parent
ayush@Legion:~/Ayush_I3104$ gcc Ass2b2.c -o child
ayush@Legion:~/Ayush_I3104$ ./parent ./child 5 3 4 9 8
[Parent] Original array:
5 3 4 9 8 
[Parent] Sorted ascending:
3 4 5 8 9 
[Child forked] PID=3463, calling execv...
[Running Second code in child's image.See they have same PID] PID=3463
[Child] Received (ascending):
3 4 5 8 9 
[Child] Sorted descending:
9 8 5 4 3 
[Parent] Child finished.
ayush@Legion:~/Ayush_I3104$
*/
