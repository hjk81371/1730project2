#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void outc(int bytes, int fd);
void outn(int lines, int fd);
void st_out();

int main(int argc, char* argv[]) {
    if (argc < 2) {
        st_out();
    } else {
        for (int i = 1; i < argc; i++) {
            if (strcmp(argv[i], "-c") == 0) {
                int bytes = atoi(argv[i + 1]);
                for (int j = i + 1; j < argc; j++) {
                    if (strcmp(argv[j], "-") == 0) {
                        st_out();
                    } else {
                        int fd = open(argv[j], O_RDONLY, 0777);
                        if (fd > 0) {
                            outc(bytes, fd);
                        } // if
                    } // if
                } // for
                break;
            } else if (strcmp(argv[i], "-n") == 0) {
                int lines = atoi(argv[i + 1]);
                for (int j = i; j < argc; j++) {
                    if (strcmp(argv[j], "-") == 0) {
                        st_out();
                    } else {
                        int fd = open(argv[j], O_RDONLY, 0777);
                        if (fd > 0) {
                            outn(lines, fd);
                        } // if
                    } // if
                } // for
                break;
            } else {
                for (int j = i; j < argc; j++) {
                    if (strcmp(argv[j], "-") == 0) {
                        st_out();
                    } else {
                        int fd = open(argv[j], O_RDONLY, 0777);
                        if (fd > 0) {
                            outc(10, fd);
                        } // if
                    } // if
                } // for
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

void st_out() {
    char buf[256];
    int length = 1;
    while (length > 0) {
        length = read(STDIN_FILENO, buf, 256);
        write(STDOUT_FILENO, buf, length);
    } // while
} // std_out
