#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char **argv) {
    if (argc != 2) return 1;
    int N = atoi(argv[1]);
    if (N <= 0) return 1;

    for (int i = 1; i <= N; i++) {
      
        char buf[32];
        int len = snprintf(buf, sizeof buf, "%d%c", i, (i < N) ? ' ' : '\n');
        write(STDOUT_FILENO, buf, (size_t)len);

        if (i < N) {
         
  
            pid_t pid = fork();
            if (pid < 0) { perror("fork"); return 1; }
            if (pid == 0) {
                continue;            
            } else {
                waitpid(pid, NULL, 0);
                break;             
            }
        }
    }
    return 0;
}
