#include "client.h"
#include <locale.h>

extern t_server server;

static void send_signal(int signal)
{
	size_t	i;

	server.waiting_for_signal = true;
	if (kill(server.pid, signal) != 0)
		print_and_exit("Error: Can't contact server, please check the PID.");
	i = 0;
	while (server.waiting_for_signal
	&& i++ < CONFIRMATION_LIMIT)
		usleep(CONFIRMATION_PERIOD_US);
	if (server.waiting_for_signal)
		print_and_exit("Error: Client doesn't received confirmation in time.");
}

static void signal_received(int signal, siginfo_t * infos, void * ucontext_t)
{
	(void)signal;
	(void)ucontext_t;
	if (infos->si_pid == server.pid) {
		server.waiting_for_signal = false;
		char * s = "server received signal\n";
		prints(2, s);
	}
}

static void usage_and_exit()
{
	printf("Usage ./client PID MESSAGE\n");
	exit(EXIT_FAILURE);
}

static void send_char(wchar_t msg)
{
	printf("send_char: %lc\n", msg);
	send_signal(SIGUSR1);
	send_signal(SIGUSR2);
}

static void send_message(wchar_t * msg)
{
	size_t i;

	i = 0;
	if (msg[i])
		send_char(msg[i++]);
}

int main(int ac, char ** av)
{
	struct sigaction sa;

	if (ac != 3)
		usage_and_exit();
	setlocale(LC_ALL, "");
	printf ("CLIENT PID: %i\n", getpid());
	server.pid = parse_pid(av[1]);
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = signal_received;
	sigaction(SIGUSR1, &sa, NULL);
	send_message(L"f");
	return (0);
}