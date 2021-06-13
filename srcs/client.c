#include "client.h"

t_server	g_server;

static void	send_signal(int signal)
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
		print_and_exit("Error: Client didn't receive confirmation in time");
	usleep(100);
}

static void	signal_received(int signal, siginfo_t *infos, void *ucontext)
{
	(void)signal;
	(void)ucontext;
	if (infos->si_pid == g_server.pid)
		g_server.waiting_for_signal = false;
}

static void	send_message(char const *msg, int msg_size)
{
	size_t	i;
	size_t	msg_i;

	i = -1;
	while (++i < sizeof(int) * 8)
	{
		if (msg_size & 1 << i)
			send_signal(SIGUSR1);
		else
			send_signal(SIGUSR2);
	}
	msg_i = -1;
	while (msg[++msg_i])
	{
		i = -1;
		while (++i < 8)
		{
			if (msg[msg_i] & 1 << i)
				send_signal(SIGUSR1);
			else
				send_signal(SIGUSR2);
		}
	}
}

int	main(int ac, char **av)
{
	struct sigaction	sa;
	int					msg_size;

	if (ac != 3)
		print_and_exit("Usage ./client PID MESSAGE");
	g_server.pid = parse_pid(av[1]);
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = signal_received;
	sigaction(SIGUSR1, &sa, NULL);
	msg_size = slen(av[2]);
	if (msg_size)
		send_message(av[2], msg_size);
	return (0);
}
