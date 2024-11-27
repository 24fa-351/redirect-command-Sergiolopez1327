#ifndef REDIRECT_H
#define REDIRECT_H

void redirect_io(const char *inp, const char *out);
void split_command(const char *cmd, char **argv);

#endif 