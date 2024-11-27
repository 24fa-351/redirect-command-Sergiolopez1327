#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "redirect.h"

void redirect(const char *input, const char *out) {
    if (strcmp(input, "-") != 0) {
        int fd_in = open(input, O_RDONLY);
        if (fd_in < 0) {
            perror("open input file");
            exit(EXIT_FAILURE);
        }
        dup2(fd_in, STDIN_FILENO);
        close(fd_in);
    }

    if (strcmp(out, "-") != 0) {
        int fd_out = open(out, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
        if (fd_out < 0) {
            perror("open output file");
            exit(EXIT_FAILURE);
        }
        dup2(fd_out, STDOUT_FILENO);
        close(fd_out);
    }
}
