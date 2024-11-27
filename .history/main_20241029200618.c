#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include "redirect.h"

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <inp> <cmd> <out>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *inp = argv[1];
    const char *cmd = argv[2];
    const char *out = argv[3];

    // Allocate array for command arguments
    char *cmd_argv[256];
    split_command(cmd, cmd_argv);

    // Get the absolute path of the command
    char *cmd_path = absolute_path(cmd_argv[0]);
    if (cmd_path == NULL) {
        fprintf(stderr, "Command not found: %s\n", cmd_argv[0]);
        exit(EXIT_FAILURE);
    }

    // Fork the process
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        free(cmd_path); // Free memory on error
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { // Child process
        redirect(inp, out);
        execv(cmd_path, cmd_argv); // Use execv here
        perror("execv"); // Only reached if execv fails
        free(cmd_path); // Free memory before exit
        exit(EXIT_FAILURE);
    } else { // Parent process
        int status;
        waitpid(pid, &status, 0);
        free(cmd_path); // Free memory in parent as well
        if (WIFEXITED(status)) {
            return WEXITSTATUS(status);
        } else {
            return EXIT_FAILURE;
        }
    }
}
