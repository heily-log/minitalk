/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakobaya <hakobaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 02:05:34 by hakobaya          #+#    #+#             */
/*   Updated: 2023/11/08 06:18:36 by hakobaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

static int	g_received;

void	timeout(void)
{
	ft_putstr_fd("\n--- send char failure ---\n\n", 1);
	exit(1);
}

void	send_char(const pid_t pid, char c)
{
	int	digit;
	int	time;

	digit = 7;
	time = 5;
	g_received = 0;
	while (digit >= 0)
	{
		usleep(200);
		if (1 & (c >> digit))
			kill(pid, SIGUSR1);
		else
			kill(pid, SIGUSR2);
		digit--;
	}
	while (g_received == 0 && time <= 4000)
		usleep(time++);
	if (time >= 4000)
		timeout();
	if (g_received == 1)
		ft_putstr_fd("send char successed !!\n", 1);
}

void	send_string(const pid_t pid, char *str)
{
	while (*str)
	{
		send_char(pid, *str);
		str++;
	}
}

void	signal_handler_client(int signum, siginfo_t *info, void *context)
{
	(void)info;
	(void)context;
	if (signum == SIGUSR1)
		g_received = 1;
}

int	main(int ac, char **av)
{
	struct sigaction	act;
	pid_t				pid;

	if (ac != 3)
	{
		ft_putstr_fd("Three arguments are valid\n", 1);
		exit(0);
	}
	pid = (const pid_t)ft_atoi(av[1]);
	if (pid < 100 || pid > 99998)
		exit(1);
	ft_memset(&act, 0, sizeof(struct sigaction));
	act.sa_sigaction = signal_handler_client;
	act.sa_flags = SA_SIGINFO;
	sigaction(SIGUSR1, &act, NULL);
	send_string((const pid_t)pid, av[2]);
	return (0);
}
