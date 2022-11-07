#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void outc(int bytes, int fd);
void outn(int lines, int fd);
void st_out(int c, int bytes);

int main(int argc, char* argv[]) {
    if (argc < 2) {
        st_out(0, 10);
    } else {
        for (int i = 1; i < argc; i++) {
            if (strcmp(argv[i], "-c") == 0) {
                int bytes = atoi(argv[i + 1]);
                for (int j = i + 1; j < argc; j++) {
                    if (strcmp(argv[j], "-") == 0) {
                        st_out(1, bytes);
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
                        st_out(0, lines);
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
                        st_out(1, 10);
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
    int end = lseek(fd, 0, SEEK_END);
    char buf[end];
    char nwln = '\n';
    lseek(fd, (end - bytes), SEEK_SET);
    read(fd, buf, (end - bytes));
    write(STDOUT_FILENO, buf, (bytes + 1));
    write(STDOUT_FILENO, &nwln, 1);
} // outc

void outn(int lines, int fd) {
    int index = 0;
    int bytes = 0;
    int end = lseek(fd, 0, SEEK_END);
    char* buf = calloc(end, sizeof(char));
    lseek(fd, 0, SEEK_SET);
    read(fd, buf, end);
    for (int i = end; i > 0; i--) {
        if (buf[i] == '\n') {
            if (index >= lines) {
                break;
            } // if
            index++;
        }
        bytes++;
    } // for
    lseek(fd, (-1*bytes), SEEK_END);
    read(fd, buf, bytes);
//    buf[end] = '\n';
    write(STDOUT_FILENO, buf, bytes - 1);
} // outn

void st_out(int c, int bytes) {
    int fd = creat("log", 0777);
    printf("fd: %d\n", fd);
    char buf[256];
    int length = 1;
    while (length > 0) {
        length = read(STDIN_FILENO, buf, 256);
        write(fd, buf, length);
    } // while
    if (c) {
        outc(bytes, -1);
    } else {
        printf("n\n");
        outn(bytes, -1);
    } // if
//    remove("log");
} // std_out
