#include <stdio.h>
#include <unistd.h>
#include <time.h>



int main(void) {
    int i;
    time_t end_sec, start_sec;

    start_sec = time(NULL);

    for (i = 1; i <= 60; i++) {
        sleep(1);
        printf("%d\n", i);
    }
    end_sec = time(NULL);
    printf("Elapsed: %ld seconds\n", end_sec - start_sec);
}
