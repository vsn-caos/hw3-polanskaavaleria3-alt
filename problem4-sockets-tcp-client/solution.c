#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

static int write_all(int fd, const void *buf, size_t n) {
    const char *p = buf;
    while (n) {
        ssize_t w = write(fd, p, n);
        if (w <= 0) return -1;
        p += w; n -= (size_t)w;
    }
    return 0;
}

static int read_all(int fd, void *buf, size_t n) {
    char *p = buf;
    while (n) {
        ssize_t r = read(fd, p, n);
        if (r == 0) return 0;
        if (r < 0) return -1;
        p += r; n -= (size_t)r;
    }
    return 1;
}

int main(int argc, char **argv) {
    if (argc != 3) return 1;

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) { perror("socket"); return 1; }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof addr);
    addr.sin_family = AF_INET;
    addr.sin_port = htons((uint16_t)atoi(argv[2]));
    if (inet_pton(AF_INET, argv[1], &addr.sin_addr) != 1) return 1;
    if (connect(fd, (struct sockaddr *)&addr, sizeof addr) < 0) {
        perror("connect"); close(fd); return 1;
    }

    long val;
    while (scanf("%ld", &val) == 1) {
        int32_t v = (int32_t)val;
        unsigned char b[4];                    
        b[0] = (unsigned char)(v);
        b[1] = (unsigned char)(v >> 8);
        b[2] = (unsigned char)(v >> 16);
        b[3] = (unsigned char)(v >> 24);
        if (write_all(fd, b, 4) < 0) break;

        unsigned char r[4];
        int st = read_all(fd, r, 4);
        if (st == 0) { close(fd); return 0; }   
        if (st < 0) break;

        int32_t resp = (int32_t)((uint32_t)r[0]
                               | ((uint32_t)r[1] << 8)
                               | ((uint32_t)r[2] << 16)
                               | ((uint32_t)r[3] << 24));
        printf("%d\n", resp);
        fflush(stdout);
    }

    close(fd);
    return 0;
}
