#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    int c;
    int l;
    int w;
    int noarg = 1;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-c") == 0) {
            c = 1;
            noarg = 0;
        } else if (strcmp(argv[i], "-l") == 0) {
            l = 1;
            noarg = 0;
        } else if(strcmp(argv[i], "-w") == 0) {
            w = 1;
            noarg = 0;
        } else {
            for (int j = 0; argv[i][j] != 0; j++) {
                if (argv[i][j] == '-') {
                    for (int k = j + 1; argv[i][k] != 0; k++) {
                        if (argv[i][k] == 'c') {
                            c = 1;
                            noarg = 0;
                        } else if (argv[i][k] == 'l') {
                            l = 1;
                            noarg = 0;
                        } else if (argv[i][k] == 'w') {
                            w = 1;
                            noarg = 0;
                        } // if
                    } // for
                } // if
            } // for
        } // if
    } // for


} // main

void std_out(fd, c, l, w, noarg) {
    char stats[4];
    stats[0] = bytes(fd);
    stats[1] = newlines(fd, '\n');
    stats[2] = words(fd);
    if (noarg) {

    } // if
} // std_out

int bytes(fd) {
    int bytes = lseek(fd, 0, SEEK_END);
    return bytes;
} // bytes

int newlines(fd, char stop) {
    int count;
    int index;
    int size = 100;
    char * buf = calloc(sizeof(int), size);
    while (read(fd, buf, bytes(fd)) > 0) {
        if (buf[index] == char) {
            count++;
        } // if
        index++;
        if (index > size - 1) {
            size = size*2;
            buf = realloc(buf, size);
        } // if
    } // while
    return count;
} // newlines

int words(fd) {
    return newlines(fd, ' ');
} // words
