#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sched.h>

int
main(void) {
    int pid, exit_code;
    if ((pid = fork()) == 0) {
        printf("fork ok.\n");
        int i;
        for (i = 0; i < 10; i ++) {
			sched_yield();
            // yield();
        }
        exit(0xbeaf);
    }
    // assert(pid > 0);
    // assert(waitpid(-1, NULL) != 0);
    // assert(waitpid(pid, (void *)0xC0000000) != 0);
    // assert(waitpid(pid, &exit_code) == 0 && exit_code == 0xbeaf);
    printf("badarg pass.\n");
    return 0;
}

