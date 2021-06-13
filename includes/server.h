#ifndef SERVER_H
# define SERVER_H
# include <unistd.h>
# include <stdbool.h>
# include <signal.h>
# include <stdlib.h>
# include "utils.h"

typedef struct s_message
{
	bool	waiting_for_size;
	int		current_char_i;
	char	*buffer;
	int		buffer_i;
	int		size;
	int		size_i;
	int		client_pid;
}				t_message;

#endif