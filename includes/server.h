#ifndef SERVER_H
# define SERVER_H
# include <unistd.h>
# include <stdbool.h>
# include <signal.h>
# include <stdlib.h>
# include "utils.h"

typedef struct	s_message
{
	bool	waiting_for_connection;
	bool	waiting_for_size;
	size_t	current_char_i;
	char *	buffer;
	size_t	buffer_i;
	size_t	size;
	size_t	size_i;
	int		client_pid;
}				t_message;

#endif