#ifndef CLIENT_H
# define CLIENT_H
# include <stdlib.h>
# include <signal.h>
# include <stdbool.h>
# include <unistd.h>
# include "utils.h"

# define CONFIRMATION_PERIOD 1000
# define DELAY_BETWEEN_SIGNALS 50

typedef struct s_server
{
	int		pid;
	bool	waiting_for_signal;
}				t_server;

#endif