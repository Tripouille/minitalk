#include "client.h"

t_server g_server;

static void send_signal(int signal)
{
	size_t	i;

	g_server.waiting_for_signal = true;
	if (kill(g_server.pid, signal) != 0)
		print_and_exit("Error: Can't contact server, please check the PID");
	i = 0;
	while (g_server.waiting_for_signal
	&& i++ < CONFIRMATION_LIMIT)
		usleep(CONFIRMATION_PERIOD_US);
	if (g_server.waiting_for_signal)
		print_and_exit("Error: Client doesn't received confirmation in time");
}

static void signal_received(int signal, siginfo_t * infos, void * ucontext_t)
{
	(void)signal;
	(void)ucontext_t;
	if (infos->si_pid == g_server.pid)
		g_server.waiting_for_signal = false;
}

static void send_message(char const * msg)
{
	size_t i;
	size_t msg_i;
	int size;

	i = -1;
	size = slen(msg);
	while (++i < sizeof(int) * 8)
		if (size & 1 << i)
			send_signal(SIGUSR1);
		else
			send_signal(SIGUSR2);
	msg_i = -1;
	while (msg[++msg_i])
	{
		i = -1;
		while (++i < 8)
			if (msg[msg_i] & 1 << i)
				send_signal(SIGUSR1);
			else
				send_signal(SIGUSR2);
	}
}

static void usage_and_exit()
{
	printf("Usage ./client PID MESSAGE\n");
	exit(EXIT_FAILURE);
}

int main(int ac, char ** av)
{
	struct sigaction sa;

	if (ac != 3)
		usage_and_exit();
	g_server.pid = parse_pid(av[1]);
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = signal_received;
	sigaction(SIGUSR1, &sa, NULL);
	send_message(av[2]);
	return (0);
}