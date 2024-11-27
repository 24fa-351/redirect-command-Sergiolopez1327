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
        fprintf(stderr, "Usage: %s <input> <command> <output>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *input = argv[1];
    const char *command = argv[2];
    const char *output = argv[3];

    char *command_argv[256];
    split(command, command_argv);

    char *command_path = abspath(command_argv[0]);
    if (command_path == NULL) {
        fprintf(stderr, "Command not found: %s\n", command_argv[0]);
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        free(command_path); 
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { 
        redirect(input, output);
        execve(command_path, command_argv, environ); 
        perror("execve"); 
        free(command_path);
        exit(EXIT_FAILURE);
    } else { 
        int status;
        waitpid(pid, &status, 0);
        free(command_path); 
        if (WIFEXITED(status)) {
            return WEXITSTATUS(status);
        } else {
            return EXIT_FAILURE;
        }
    }
}
