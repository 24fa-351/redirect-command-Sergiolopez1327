#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "abspath.h"
char *abspath(const char *cmd) {
    if (strchr(cmd, '/') != NULL) {
        return strdup(cmd);
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
        size_t cmd_len = strlen(token) + strlen(cmd) + 2;
        char *cmd_path = malloc(cmd_len);
        if (cmd_path == NULL) {
            free(path_copy); 
            return NULL; 
        }
        snprintf(cmd_path, cmd_len, "%s/%s", token, cmd);
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
