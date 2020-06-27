#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    pid_t pid;
    pid = fork();
    if (pid < 0) {
        perror("Wrong!");
        return -1;
    } else if (pid == 0) {
        printf("Right!");
        return 0;
    } else {
        printf("Father!");
    }

    return 0;
}

