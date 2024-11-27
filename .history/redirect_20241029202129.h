#ifndef REDIRECT_H
#define REDIRECT_H

void redirect(const char *inp, const char *out);
void split_command(const char *cmd, char **argv);
char *absolute_path(const char *cmd);

#endif 