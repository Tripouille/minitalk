#ifndef UTILS_H
# define UTILS_H
# include <stddef.h>
# include <limits.h>
# include <stdlib.h>
# include <unistd.h>

void	print_and_exit(char const *msg);
size_t	slen(char const *s);
void	prints(int fd, char const *s);
int		parse_pid(char const *s);
void	print_pid(int pid);

#endif