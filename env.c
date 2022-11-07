#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char * argv[], char * env[]) {
    for (int i = 0; env[i] != NULL; i++) {
        write(STDOUT_FILENO, *env, 1000);
    } // for
} // main
