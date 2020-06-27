#include <stdio.h>

/* version 1
int main()
{
    int c;
    c = getchar();
    while (c != EOF) {
        putchar(c);
        c = getchar();
    }
}
*/

int main()
{
    int c;
    while ((c = getchar()) != EOF) {
    //    putchar(c);
        putchar(getchar() != EOF);
    }
}
