#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include "redirect.h"

void redirect(const char *inp, const char *out) {
    if (strcmp(inp, "-") != 0) {
        int fd_in = open(inp, O_RDONLY);
        if (fd_in < 0) {
            perror("open input file");
            exit(EXIT_FAILURE);
        }
        dup2(fd_in, STDIN_FILENO);
        close(fd_in);
    }

    if (strcmp(out, "-") != 0) {
        int fd_out = open(out, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd_out < 0) {
            perror("open output file");
            exit(EXIT_FAILURE);
        }
        dup2(fd_out, STDOUT_FILENO);
        close(fd_out);
    }
}

void split_command(const char *cmd, char **argv) {
    char *cmd_copy = strdup(cmd);
    char *token = strtok(cmd_copy, " ");
    int i = 0;
    while (token != NULL) {
        argv[i++] = token;
        token = strtok(NULL, " ");
    }
    argv[i] = NULL;
}

char *absolute_path(const char *cmd) {
    if (strchr(cmd, '/') != NULL) {
        return strdup(cmd);
    }

    char *path = getenv("PATH");
    if (path == NULL) {
        return NULL;
    }

    char *path_copy = strdup(path);
    char *token = strtok(path_copy, ":");
    while (token != NULL) {
        char *cmd_path = malloc(strlen(token) + strlen(cmd) + 2);
        sprintf(cmd_path, "%s/%s", token, cmd);
        if (access(cmd_path, X_OK) == 0) {
            free(path_copy);
            return cmd_path;
        }
        free(cmd_path);
        token = strtok(NULL, ":");
    }
    free(path_copy);
    return NULL;
}
