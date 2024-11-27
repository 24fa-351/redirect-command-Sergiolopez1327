#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "redirect.h"

void redirect(const char *input, const char *output) {
    if (strcmp(input, "-") != 0) {
        int fd_in = open(input, O_RDONLY);
        if (fd_in < 0) {
            perror("open input file");
            exit(EXIT_FAILURE);
        }
        dup2(fd_in, STDIN_FILENO);
        close(fd_in);
    }

    if (strcmp(output, "-") != 0) {
        int fd_output = open(output, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
        if (fd_output < 0) {
            perror("open outputput file");
            exit(EXIT_FAILURE);
        }
        dup2(fd_output, STDoutput_FILENO);
        close(fd_output);
    }
}

