#include "server.h"

t_message	g_message;

static void	reset_message(int client_pid)
{
	g_message.waiting_for_size = true;
	g_message.current_char_i = 0;
	free(g_message.buffer);
	g_message.buffer = NULL;
	g_message.buffer_i = 0;
	g_message.size = 0;
	g_message.size_i = 0;
	g_message.client_pid = client_pid;
}

static void	parse_message_size(bool current_bit_is_true)
{
	int		i;

	if (current_bit_is_true)
		g_message.size |= 1 << g_message.size_i;
	++g_message.size_i;
	if (g_message.size_i == sizeof(int) * 8)
	{
		if (g_message.size)
		{
			g_message.waiting_for_size = false;
			g_message.buffer = malloc(g_message.size);
			if (g_message.buffer == NULL)
				print_and_exit("Error: Out of memory");
			i = 0;
			while (i < g_message.size)
				g_message.buffer[i++] = 0;
		}
		else
			reset_message(0);
	}
}

static void	parse_message(bool current_bit_is_true)
{
	if (current_bit_is_true)
		g_message.buffer[g_message.buffer_i] |= 1 << g_message.current_char_i;
	++g_message.current_char_i;
	if (g_message.current_char_i == 8)
	{
		g_message.current_char_i = 0;
		++g_message.buffer_i;
		if (g_message.buffer_i == g_message.size)
		{
			write(1, g_message.buffer, g_message.size);
			write(1, "\n", 1);
			reset_message(0);
		}
	}
}

static void	handle_message(int signal, siginfo_t *infos, void *ucontext)
{
	(void)ucontext;
	if (g_message.client_pid != infos->si_pid)
		reset_message(infos->si_pid);
	if (g_message.waiting_for_size)
		parse_message_size(signal == SIGUSR1);
	else
		parse_message(signal == SIGUSR1);
	kill(infos->si_pid, SIGUSR1);
}

int	main(void)
{
	struct sigaction	sa;

	print_pid(getpid());
	write(1, "\n", 1);
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = handle_message;
	sigaction(SIGUSR1, &sa, NULL);
	sigaction(SIGUSR2, &sa, NULL);
	while (true)
		pause();
	return (0);
}
