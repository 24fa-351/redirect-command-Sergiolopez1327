#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include "redirect.h"
#include "abspath.h"
#include "split.h"

extern char **environ;

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <inp> <cmd> <out>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *inp = argv[1];
    const char *cmd = argv[2];
    const char *out = argv[3];

    char *cmd_argv[256];
    split(cmd, cmd_argv);

    char *cmd_path = abspath(cmd_argv[0]);
    if (cmd_path == NULL) {
        fprintf(stderr, "Command not found: %s\n", cmd_argv[0]);
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        free(cmd_path); 
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { 
        redirect(inp, out);
        execve(cmd_path, cmd_argv, environ); 
        perror("execve"); 
        free(cmd_path);
        exit(EXIT_FAILURE);
    } else { 
        int status;
        waitpid(pid, &status, 0);
        free(cmd_path); 
        if (WIFEXITED(status)) {
            return WEXITSTATUS(status);
        } else {
            return EXIT_FAILURE;
        }
    }
}
