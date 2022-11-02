#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void outc(int bytes, int fd);
void outn(int lines, int fd);

int main(int argc, char* argv[]) {
    if (argc < 2) {
        // case for no command line arguments
    } else {
        for (int i = 1; i < argc; i++) {
            if (strcmp(argv[i], "-c") == 0) {
                int bytes = atoi(argv[i + 1]);
                for (int j = i + 1; j < argc; j++) {
                    int fd = open(argv[j], O_RDONLY, 0777);
                    if (fd > 0) {
                        outc(bytes, fd);
                    } // if
                } // for
                break;
            } else if (strcmp(argv[i], "-n") == 0) {
                int lines = atoi(argv[i + 1]);
                for (int j = i + 1; j < argc; j++) {
                    int fd = open(argv[j], O_RDONLY, 0777);
                    if (fd > 0) {
                        outn(lines, fd);
                    } // if
                } // for
                break;
            } else {
                    int fd = open(argv[i], O_RDONLY, 0777);
                    if (fd > 0) {
                        int length = lseek(fd, 0, SEEK_END);
                        outc(length, fd);
                    } // if
            } // if
            break;
        } // for
    } // if
} // main

void outc(int bytes, int fd) {
    char buf[bytes + 1];
    lseek(fd, 0, SEEK_SET);
    read(fd, buf, bytes);
    buf[bytes] = '\n';
    write(STDOUT_FILENO, buf, bytes + 1);
} // outc

void outn(int lines, int fd) {
    int index = 1;
    int bytes = 0;
    char buf[lines*105];
    int end = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);
    read(fd, buf, end);
    for (int i = 0; i < end; i++) {
        if (buf[i] == '\n') {
            if (index >= lines) {
                break;
            } // if
            index++;
        }
        bytes++;
    } // for
    buf[bytes] = '\n';
    write(STDOUT_FILENO, buf, bytes + 1);
} // outn
