#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void total(int c, int l, int w, int noarg, int * total_lines, int * total_words, int * total_bytes);
void stdio(int c, int w, int l, int noarg, int * lineptr, int * wordptr, int * byteptr);
void std_out(int fd, int c, int l, int w, int noarg, int * lineptr, int * wordptr, int * byteptr);
int bytes(int fd, int * total_bytes);
int newlines(int fd, char stop, int * total_lines);
int words(int fd, int * total_words);
char * to_char_array(int num, int * arr_length);

int main(int argc, char* argv[]) {
    int c = 0;
    int l = 0;
    int w = 0;
    int noarg = 1;
    int files = 0;

    int total_lines = 0;
    int total_words = 0;
    int total_bytes = 0;

    int * lineptr = &total_lines;
    int * wordptr = &total_words;
    int * byteptr = &total_bytes;

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
        } else if (strcmp(argv[i], "-") == 0) {
            files++;
            stdio(c, w, l, noarg, lineptr, wordptr, byteptr);
        } else {
            int fd = open(argv[i], O_RDONLY, 0777);
           if (fd > 0) {
               files++;
               char nwln = '\n';
               int lngth = 0;
               while(argv[i][++lngth] > 32);
               std_out(fd, c, l, w, noarg, lineptr, wordptr, byteptr);
               write(STDOUT_FILENO, argv[i], lngth);
               write(STDOUT_FILENO, &nwln, 1);
            } else {
                for (int j = 0; argv[i][j] != 0; j++) {
                    if (argv[i][j] == '-') {
                        if (argv[i][j + 1] < 33) {
                            stdio(c, w, l, noarg, lineptr, wordptr, byteptr);
                            char nwln = '\n';
                            int lngth = 0;
                            while(argv[i][++lngth] > 32);
                            write(STDOUT_FILENO, argv[i], lngth);
                            write(STDOUT_FILENO, &nwln, 1);
                        } else {
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
                    } // if
                } // for
            } // if
        } // if
    } // for
    if (files > 1) {
        total(c, l, w, noarg, lineptr, wordptr, byteptr);
    } // if

} // main

void stdio(int c, int w, int l, int noarg, int * lineptr, int * wordptr, int * byteptr) {
    char buf[512];
    int length = 1;
    int linecount = 0;
    int wordcount = 0;
    int bytecount = 0;

//    *wordptr /= 2;

    while (length > 0) {
        length = read(STDIN_FILENO, buf, 512);
        if (length <= 0) break;
        bytecount += length;
        *byteptr += length;

        for (int i = 0; i < length; i++) {
            if (buf[i] == '\n') {
                linecount++;
                *lineptr += 1;
                wordcount++;
                *wordptr += 1;
            } else if (buf[i] == ' ') {
                wordcount++;
                *wordptr += 1;
            } // if
        } // for
    } // while

    int line_length = 0;
    int word_length = 0;
    int byte_length = 0;
    int * linelengthptr = &line_length;
    int * wordlengthptr = &word_length;
    int * bytelengthptr = &byte_length;
    char * line = to_char_array(linecount, linelengthptr);
    char * word = to_char_array(wordcount, wordlengthptr);
    char * byte = to_char_array(bytecount, bytelengthptr);
    char dspace = 9;
    char nwln = '\n';

       if (noarg) {
        for (int i = 0; i < line_length; i++) {
            write(STDOUT_FILENO, (line + i), sizeof(char));
        }  //for
        write(STDOUT_FILENO, &dspace, 1);
        for (int i = 0; i < word_length; i++) {
            write(STDOUT_FILENO, (word + i), sizeof(char));
        }  //for
        write(STDOUT_FILENO, &dspace, 1);
        for (int i = 0; i < byte_length; i++) {
            write(STDOUT_FILENO, (byte + i), sizeof(char));
        }  //for
        write(STDOUT_FILENO, &dspace, 1);
    } else if (c & l & w) {
        for (int i = 0; i < line_length; i++) {
            write(STDOUT_FILENO, (line + i), sizeof(char));
        }  //for
        write(STDOUT_FILENO, &dspace, 1);
        for (int i = 0; i < word_length; i++) {
            write(STDOUT_FILENO, (word + i), sizeof(char));
        }  //for
        write(STDOUT_FILENO, &dspace, 1);
        for (int i = 0; i < byte_length; i++) {
            write(STDOUT_FILENO, (byte + i), sizeof(char));
        }  //for
        write(STDOUT_FILENO, &dspace, 1);
    } else if (c & l) {
        for (int i = 0; i < line_length; i++) {
            write(STDOUT_FILENO, (line + i), sizeof(char));
        }  //for
        write(STDOUT_FILENO, &dspace, 1);
        for (int i = 0; i < byte_length; i++) {
            write(STDOUT_FILENO, (byte + i), sizeof(char));
        }  //for
        write(STDOUT_FILENO, &dspace, 1);
    } else if (c & w) {
        for (int i = 0; i < word_length; i++) {
            write(STDOUT_FILENO, (word + i), sizeof(char));
        }  //for
        write(STDOUT_FILENO, &dspace, 1);
        for (int i = 0; i < byte_length; i++) {
            write(STDOUT_FILENO, (byte + i), sizeof(char));
        }  //for
        write(STDOUT_FILENO, &dspace, 1);
    } else if (l & w) {
        for (int i = 0; i < line_length; i++) {
            write(STDOUT_FILENO, (line + i), sizeof(char));
        }  //for
        write(STDOUT_FILENO, &dspace, 1);
        for (int i = 0; i < word_length; i++) {
            write(STDOUT_FILENO, (word + i), sizeof(char));
        }  //for
        write(STDOUT_FILENO, &dspace, 1);
    } else if (c) {
        for (int i = 0; i < byte_length; i++) {
            write(STDOUT_FILENO, (byte + i), sizeof(char));
        }  //for
        write(STDOUT_FILENO, &dspace, 1);
    } else if (l) {
        for (int i = 0; i < line_length; i++) {
            write(STDOUT_FILENO, (line + i), sizeof(char));
        }  //for
        write(STDOUT_FILENO, &dspace, 1);
    } else if (w) {
        for (int i = 0; i < word_length; i++) {
            write(STDOUT_FILENO, (word + i), sizeof(char));
        }  //for
        write(STDOUT_FILENO, &dspace, 1);
    } // if
       char dash = '-';
       write(STDOUT_FILENO, &dash, 1);
       write(STDOUT_FILENO, &nwln, 1);

} // stdio

void total(int c, int l, int w, int noarg, int * total_lines, int * total_words, int * total_bytes){
    int line_length = 0;
    int word_length = 0;
    int byte_length = 0;

    int * lineptr = &line_length;
    int * wordptr = &word_length;
    int * byteptr = &byte_length;

    int temp = ceil(*total_words / 2);
    total_words = &temp;

    char* line = to_char_array(*total_lines, lineptr);
    char* word = to_char_array(*total_words, wordptr);
    char* byte = to_char_array(*total_bytes, byteptr);

    char total[] = "total";
    char dspace = 9;
    char nwln = '\n';


    if (noarg) {
        for (int i = 0; i < line_length; i++) {
            write(STDOUT_FILENO, (line + i), sizeof(char));
        }  //for
        write(STDOUT_FILENO, &dspace, 1);
        for (int i = 0; i < word_length; i++) {
            write(STDOUT_FILENO, (word + i), sizeof(char));
        }  //for
        write(STDOUT_FILENO, &dspace, 1);
        for (int i = 0; i < byte_length; i++) {
            write(STDOUT_FILENO, (byte + i), sizeof(char));
        }  //for
        write(STDOUT_FILENO, &dspace, 1);
    } else if (c & l & w) {
        for (int i = 0; i < line_length; i++) {
            write(STDOUT_FILENO, (line + i), sizeof(char));
        }  //for
        write(STDOUT_FILENO, &dspace, 1);
        for (int i = 0; i < word_length; i++) {
            write(STDOUT_FILENO, (word + i), sizeof(char));
        }  //for
        write(STDOUT_FILENO, &dspace, 1);
        for (int i = 0; i < byte_length; i++) {
            write(STDOUT_FILENO, (byte + i), sizeof(char));
        }  //for
        write(STDOUT_FILENO, &dspace, 1);
    } else if (c & l) {
        for (int i = 0; i < line_length; i++) {
            write(STDOUT_FILENO, (line + i), sizeof(char));
        }  //for
        write(STDOUT_FILENO, &dspace, 1);
        for (int i = 0; i < byte_length; i++) {
            write(STDOUT_FILENO, (byte + i), sizeof(char));
        }  //for
        write(STDOUT_FILENO, &dspace, 1);
    } else if (c & w) {
        for (int i = 0; i < word_length; i++) {
            write(STDOUT_FILENO, (word + i), sizeof(char));
        }  //for
        write(STDOUT_FILENO, &dspace, 1);
        for (int i = 0; i < byte_length; i++) {
            write(STDOUT_FILENO, (byte + i), sizeof(char));
        }  //for
        write(STDOUT_FILENO, &dspace, 1);
    } else if (l & w) {
        for (int i = 0; i < line_length; i++) {
            write(STDOUT_FILENO, (line + i), sizeof(char));
        }  //for
        write(STDOUT_FILENO, &dspace, 1);
        for (int i = 0; i < word_length; i++) {
            write(STDOUT_FILENO, (word + i), sizeof(char));
        }  //for
        write(STDOUT_FILENO, &dspace, 1);
    } else if (c) {
        for (int i = 0; i < byte_length; i++) {
            write(STDOUT_FILENO, (byte + i), sizeof(char));
        }  //for
        write(STDOUT_FILENO, &dspace, 1);
    } else if (l) {
        for (int i = 0; i < line_length; i++) {
            write(STDOUT_FILENO, (line + i), sizeof(char));
        }  //for
        write(STDOUT_FILENO, &dspace, 1);
    } else if (w) {
        for (int i = 0; i < word_length; i++) {
            write(STDOUT_FILENO, (word + i), sizeof(char));
        }  //for
        write(STDOUT_FILENO, &dspace, 1);
    } // if
        write(STDOUT_FILENO, total, 5);
        write(STDOUT_FILENO, &nwln, 1);
} // total

void std_out(int fd, int c, int l, int w, int noarg, int * lineptr, int * wordptr, int * byteptr) {
    // line, word, byte

    int byte_length = 0;
    int line_length = 0;
    int word_length = 0;


    int * bytptr = &byte_length;
    int * linptr = &line_length;
    int * wrdptr = &word_length;
    char* byte = to_char_array(bytes(fd, byteptr), bytptr);
    char* line = to_char_array(newlines(fd, '\n', lineptr), linptr);
    char* word = to_char_array(words(fd, wordptr), wrdptr);
    char dspace = 9;

    if (noarg) {
        for (int i = 0; i < line_length; i++) {
            write(STDOUT_FILENO, (line + i), sizeof(char));
        }  //for
        write(STDOUT_FILENO, &dspace, 1);
        for (int i = 0; i < word_length; i++) {
            write(STDOUT_FILENO, (word + i), sizeof(char));
        }  //for
        write(STDOUT_FILENO, &dspace, 1);
        for (int i = 0; i < byte_length; i++) {
            write(STDOUT_FILENO, (byte + i), sizeof(char));
        }  //for
        write(STDOUT_FILENO, &dspace, 1);
    } else if (c & l & w) {
        for (int i = 0; i < line_length; i++) {
            write(STDOUT_FILENO, (line + i), sizeof(char));
        }  //for
        write(STDOUT_FILENO, &dspace, 1);
        for (int i = 0; i < word_length; i++) {
            write(STDOUT_FILENO, (word + i), sizeof(char));
        }  //for
        write(STDOUT_FILENO, &dspace, 1);
        for (int i = 0; i < byte_length; i++) {
            write(STDOUT_FILENO, (byte + i), sizeof(char));
        }  //for
        write(STDOUT_FILENO, &dspace, 1);
    } else if (c & l) {
        for (int i = 0; i < line_length; i++) {
            write(STDOUT_FILENO, (line + i), sizeof(char));
        }  //for
        write(STDOUT_FILENO, &dspace, 1);
        for (int i = 0; i < byte_length; i++) {
            write(STDOUT_FILENO, (byte + i), sizeof(char));
        }  //for
        write(STDOUT_FILENO, &dspace, 1);
    } else if (c & w) {
        for (int i = 0; i < word_length; i++) {
            write(STDOUT_FILENO, (word + i), sizeof(char));
        }  //for
        write(STDOUT_FILENO, &dspace, 1);
        for (int i = 0; i < byte_length; i++) {
            write(STDOUT_FILENO, (byte + i), sizeof(char));
        }  //for
        write(STDOUT_FILENO, &dspace, 1);
    } else if (l & w) {
        for (int i = 0; i < line_length; i++) {
            write(STDOUT_FILENO, (line + i), sizeof(char));
        }  //for
        write(STDOUT_FILENO, &dspace, 1);
        for (int i = 0; i < word_length; i++) {
            write(STDOUT_FILENO, (word + i), sizeof(char));
        }  //for
        write(STDOUT_FILENO, &dspace, 1);
    } else if (c) {
        for (int i = 0; i < byte_length; i++) {
            write(STDOUT_FILENO, (byte + i), sizeof(char));
        }  //for
        write(STDOUT_FILENO, &dspace, 1);
    } else if (l) {
        for (int i = 0; i < line_length; i++) {
            write(STDOUT_FILENO, (line + i), sizeof(char));
        }  //for
        write(STDOUT_FILENO, &dspace, 1);
    } else if (w) {
        for (int i = 0; i < word_length; i++) {
            write(STDOUT_FILENO, (word + i), sizeof(char));
        }  //for
        write(STDOUT_FILENO, &dspace, 1);
    } // if
} // std_out

int bytes(int fd, int * total_bytes) {
    lseek(fd, 0, SEEK_SET);
    int bytes = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);
    *total_bytes += bytes;
    return bytes;
} // bytes

int newlines(int fd, char stop, int * total_lines) {
    int count = 0;
    int length = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);
    char * buf = calloc(length, sizeof(char));
    read(fd, buf, length);
    for (int i = 0; i < length; i++) {
        if (buf[i] == stop) {
            count++;
        } // if
    } // while
    *total_lines += count;
    return count;
} // newlines

int words(int fd, int * total_words) {
    int words = newlines(fd, ' ', total_words) + newlines(fd, '\n', total_words);
    *total_words += words;
    return words;
} // words


char * to_char_array(int num, int * arr_length) {
    int counter = log10(num) + 1;
    if (counter < 1) counter = 1;
    *arr_length = counter;
    int i;
    char* array = calloc(counter, sizeof(char));
    for (i = counter-1; i >= 0; --i, num /= 10) {
        array[i] = (num % 10) + '0';
    } // for
    return array;
} // to_char_array
