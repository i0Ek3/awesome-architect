#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    char str;
    scanf("%s", &str);
    
    // exit()会将缓冲区的数据写完后才退出，而_exit()函数直接退出
    if (strcmp(&str, "exit") == 0) {
        printf("I am exit!\n");
        printf("I am on the buffer!");
        exit(0);
    } else if (strcmp(&str, "_exit") == 0) {
        printf("I am _exit!\n");
        printf("I am on the buffer!");
        _exit(0);
    } else {
        abort();
    }
    return 0;
}

