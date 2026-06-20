#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>

int main(int argc, char **argv) {
    if (argc != 3) return 1;

    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) { perror("open"); return 1; }

    struct stat st;
    if (fstat(fd, &st) < 0) { perror("fstat"); close(fd); return 1; }

    size_t filesize = (size_t)st.st_size;
    size_t needle_len = strlen(argv[2]);

    if (filesize == 0 || needle_len == 0 || needle_len > filesize) {
        close(fd);
        return 0;
    }

    char *data = mmap(NULL, filesize, PROT_READ, MAP_PRIVATE, fd, 0);
    if (data == MAP_FAILED) { perror("mmap"); close(fd); return 1; }

    char *p = data;
    size_t remaining = filesize;
    while (remaining >= needle_len) {
        char *found = memmem(p, remaining, argv[2], needle_len);
        if (!found) break;
        printf("%ld\n", (long)(found - data));
        p = found + 1;                      
        remaining = filesize - (size_t)(p - data);
    }

    munmap(data, filesize);
    close(fd);
    return 0;
}
