#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char **argv) {
    if (argc != 3) return 1;

    int pipefd[2];
    if (pipe(pipefd) < 0) { perror("pipe"); return 1; }

    pid_t p1 = fork();
    if (p1 < 0) { perror("fork"); return 1; }
    if (p1 == 0) {                         
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[0]);
        close(pipefd[1]);
        execlp("sh", "sh", "-c", argv[1], (char *)NULL);
        perror("execlp cmd1");
        _exit(1);
    }

    pid_t p2 = fork();
    if (p2 < 0) { perror("fork"); return 1; }
    if (p2 == 0) {                         
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);
        close(pipefd[1]);
        execlp("sh", "sh", "-c", argv[2], (char *)NULL);
        perror("execlp cmd2");
        _exit(1);
    }


    close(pipefd[0]);
    close(pipefd[1]);
    waitpid(p1, NULL, 0);
    waitpid(p2, NULL, 0);
    return 0;
}
