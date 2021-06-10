#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include <signal.h>

static void handle_sigusr_one(int signal, siginfo_t * infos, void * ucontext_t)
{
	(void)signal;
	(void)ucontext_t;
	printf("Signal one from pid %i\n", infos->si_pid);
	kill(infos->si_pid, SIGUSR1);
}

static void handle_sigusr_two(int signal, siginfo_t * infos, void * ucontext_t)
{
	(void)signal;
	(void)infos;
	(void)ucontext_t;
	printf("Signal two from pid %i\n", infos->si_pid);
	kill(infos->si_pid, SIGUSR1);
}

int main (void)
{
	struct sigaction sa[2];

	printf ("SERVER PID: %i\n", getpid());
	sa[0].sa_flags = SA_SIGINFO;
	sa[0].sa_sigaction = handle_sigusr_one;
	sigaction(SIGUSR1, sa, NULL);
	sa[1].sa_flags = SA_SIGINFO;
	sa[1].sa_sigaction = handle_sigusr_two;
	sigaction(SIGUSR2, sa + 1, NULL);
	while (true)
		pause();
	return (0);
}