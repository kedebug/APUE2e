#include <apue.h>
#include <pwd.h>

static void myalarm(int signo) {
    struct passwd *rootptr;

    printf("In signal handler\n");
    if ((rootptr = getpwnam("root")) == NULL)
        err_sys("getpwnam(root) error\n");
    alarm(1);
}

int main() {
    struct passwd *ptr;

    signal(SIGALRM, myalarm);
    alarm(1);

    while (true) {
        if ((ptr = getpwnam("kedebug")) == NULL)
            err_sys("getpwnam(kedebug) error\n");
        if (strcmp(ptr->pw_name, "kedebug") != 0)
            printf("return value corrupted, pw_name=%s\n", 
                    ptr->pw_name);
    }
}
