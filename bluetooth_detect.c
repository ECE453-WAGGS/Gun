#include <stdbool.h>
#include <stdio.h>

int main()
{
    while (true) {
        printf("FIRE");
        fflush(stdout);

        sleep(1);
    }

    return 0;
}
