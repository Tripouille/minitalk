#ifndef CLIENT_H
# define CLIENT_H
# include <stdlib.h>
# include <stdio.h>
# include <signal.h>
# include <stdbool.h>
# include <unistd.h>
# include "utils.h"

# define CONFIRMATION_PERIOD_US 1000
# define CONFIRMATION_LIMIT 10

typedef	struct	s_server
{
	int		pid;
	bool	waiting_for_signal;
}				t_server;

#endif