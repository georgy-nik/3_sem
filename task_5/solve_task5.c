#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

char* data = NULL;
int arr = 0;
int qwe = 0;
int cpid, ppid;

int size = 8;

char* input = "solve_task5.c";
char* output = "solve_task5_copy.c";

char mask() {
    return 1 << (arr % 8);
}
void expand(int d) {
    static int data_size = 0;
    if(data_size == 0) {
        data = malloc(1);
        data_size = 1;
    }
    while(d < data_size)
        data = realloc(data, data_size *= 2);
}
void set_zero(int n) {
    expand(arr / 8 + 1);
    data[arr / 8] &= ~mask();
    arr++;
    kill(ppid, SIGUSR1);
}
void set_one(int n) {
    expand(arr / 8 + 1);
    data[arr / 8] |= mask();
    arr++;
    kill(ppid, SIGUSR1);
}
void finish(int n) {
    int fd = open(output, O_CREAT | O_WRONLY, 0600);
    write(fd, data, arr / 8);
    exit(0);
}
void send_bit(int n) {
    if(arr / 8 == qwe)
        return;
    if(data[arr / 8] & mask()) {
        arr++;
        kill(cpid, SIGUSR2);
    }
    else {
        arr++;
        kill(cpid, SIGUSR1);
    }
}
int main() {
    signal(SIGUSR1, set_zero);
    signal(SIGUSR2, set_one);
    signal(SIGINT, finish);

    ppid = getpid();
    cpid = fork();

    if(cpid) {
        signal(SIGUSR1, send_bit);
        signal(SIGINT, SIG_DFL);
        int fd = open(input, O_RDONLY);

        expand(size);
        for(int d; d = read(fd, &data[qwe], size); qwe += d)
            expand(qwe + size);
        write(STDOUT_FILENO, data, qwe);
        send_bit(0);
        while(arr / 8 != qwe)
            usleep(1);
        kill(cpid, SIGINT);
    }
    else
        while(1) pause();
}
