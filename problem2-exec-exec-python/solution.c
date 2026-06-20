#include <stdio.h>
#include <unistd.h>

int main(void) {
   
    execlp("python3", "python3", "-c",
           "import sys; print(eval(sys.stdin.read()))", (char *)NULL);
    perror("execlp");
    return 1;
}
