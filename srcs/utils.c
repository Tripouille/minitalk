#include "utils.h"

void print_and_exit(char const * msg)
{
	prints(2, msg);
	exit(EXIT_FAILURE);
}

size_t slen(char const * s)
{
	size_t len;

	len = 0;
	while (s[len])
		++len;
	return (len);
}

void prints(int fd, char const * s)
{
	write(fd, s, slen(s));
}

int parse_pid(char const * s)
{
	size_t	i;
	int pid;

	i = 0;
	pid = 0;
	while (s[i] && s[i] >= '0' && s[i] <= '9'
	&& pid * 10 + s[i] - '0' > pid && pid * 10 + s[i] - '0' <= INT_MAX)
		pid = pid * 10 + s[i++] - '0';
	if (s[i])
		print_and_exit("Error: Invalid PID");
	return (pid);
}
