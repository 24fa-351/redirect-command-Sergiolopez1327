#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "abspath.h"

char *abspath(const char *command) {
    if (strchr(command, '/') != NULL) {
        return strdup(command);
    }
    char *path = getenv("PATH");
    if (path == NULL) {
        return NULL;
    }
    char *path_copy = strdup(path);
    if (path_copy == NULL) {
        return NULL; 
    }
    char *token = strtok(path_copy, ":");
    while (token != NULL) {
        size_t command_len = strlen(token) + strlen(command) + 2;
        char *command_path = malloc(command_len);
        if (command_path == NULL) {
            free(path_copy); 
            return NULL; 
        }
        snprintf(command_path, command_len, "%s/%s", token, command);
        if (access(command_path, X_OK) == 0) {
            free(path_copy);
            return command_path;
        }
        free(command_path); 
        token = strtok(NULL, ":");
    }
    free(path_copy);
    return NULL; 
}
