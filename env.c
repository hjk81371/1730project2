#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char * argv[], char * env[]) {
    for (int i = 0; env[i] != NULL; i++) {
        printf("%s\n", env[i]);
    } // for
} // main
