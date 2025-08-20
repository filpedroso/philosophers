/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 15:07:25 by fpedroso          #+#    #+#             */
/*   Updated: 2025/08/20 23:51:10 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	simulation(t_rules rules)
{
	pid_t	*pids;

	pids = (pid_t *)malloc(sizeof(pid_t) * rules.number_of_philosophers);
	if (!pids)
		return (1);
	rules.forks = sem_open("forks", O_CREAT, 0644,
			rules.number_of_philosophers);
	rules.print_semaphore = sem_open("printer", O_CREAT, 0644, 1);
	rules.start_time = time_now_ms();
	create_processes(&rules, pids);
	reap_processes(&rules, pids);
	free(pids);
	return (0);
}

void	create_processes(t_rules *rules, pid_t *pids)
{
	int		i;
	pid_t	pid;

	i = -1;
	while (++i < rules->number_of_philosophers)
	{
		pid = fork();
		if (pid == 0)
		{
			free(pids);
			routine(rules, i + 1);
		}
		else
			pids[i] = pid;
	}
}

void	reap_processes(t_rules *rules, pid_t *pids)
{
	int			status;
	int			i;
	long long	now;

	while (1)
	{
		if (waitpid(-1, &status, 0) == -1)
			break ;
		if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
		{
			now = time_now_ms();
			i = -1;
			while (++i < rules->number_of_philosophers)
				kill(pids[i], SIGKILL);
			printf("%lld %i died\n", now - rules->start_time,
				WEXITSTATUS(status));
			i = -1;
			while (++i < rules->number_of_philosophers)
				waitpid(pids[i], NULL, 0);
			break ;
		}
	}
	unlink_and_close_semaphores(rules);
}

void	unlink_and_close_semaphores(t_rules *rules)
{
	sem_close(rules->forks);
	sem_close(rules->print_semaphore);
	sem_unlink("forks");
	sem_unlink("printer");
}
