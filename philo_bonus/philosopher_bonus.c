/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 16:43:40 by fpedroso          #+#    #+#             */
/*   Updated: 2025/08/17 16:43:40 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	main(int argc, char **argv)
{
	t_rules	rules;
	int		status;

	if ((argc != 5) && (argc != 6))
		return (1);
	if (!parse_args(argc, argv, &rules))
		return (1);
	sem_unlink("forks");
	status = 0;
	status = simulation(rules);
	return (status);
}

bool	parse_args(int argc, char **argv, t_rules *rules)
{
	memset((void *)rules, 0, sizeof(t_rules));
	rules->number_of_philosophers = atoi_positive(argv[1]);
	rules->time_to_die = atoi_positive(argv[2]);
	rules->time_to_eat = atoi_positive(argv[3]);
	rules->time_to_sleep = atoi_positive(argv[4]);
	if (argc == 6)
		rules->number_of_times_each_philosopher_must_eat = atoi_positive(argv[5]);
	else
		rules->number_of_times_each_philosopher_must_eat = NO_ARG;
	if (rules->number_of_philosophers == -1 || rules->time_to_die == -1
		|| rules->time_to_eat == -1 || rules->time_to_sleep == -1
		|| rules->number_of_times_each_philosopher_must_eat == -1)
		return (false);
	return (true);
}

int	atoi_positive(char *str)
{
	int	num;

	if (!str || !*str)
		return (-1);
	num = 0;
	while (*str)
	{
		if (*str < '0' || *str > '9')
			return (-1);
		num = num * 10 + (*str - '0');
		str++;
	}
	return (num);
}

int	simulation(t_rules rules)
{
	pid_t	*pids;

	pids = (pid_t *)malloc(sizeof(pid_t) * rules.number_of_philosophers);
	if (!pids)
		return (1);
	rules.forks = sem_open("forks", O_CREAT, 0644,
			rules.number_of_philosophers);
	rules.start_time = time_now_ms();
	create_processes(&rules, pids);
	reap_processes(&rules, pids);
	free(pids);
	return (0);
}

void	reap_processes(t_rules *rules, pid_t *pids)
{
	int	status;
	int	i;

	while (1)
	{
		if (waitpid(-1, &status, 0) == -1)
			break ;
		if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
		{
			i = -1;
			while (++i < rules->number_of_philosophers)
				kill(pids[i], SIGKILL);
			printf("%lld %i died\n", time_now_ms() - rules->start_time,
				WEXITSTATUS(status));
			i = -1;
			while (++i < rules->number_of_philosophers)
				waitpid(pids[i], NULL, 0);
			break ;
		}
	}
	sem_close(rules->forks);
	sem_unlink("forks");
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

void	routine(t_rules *rules, int id)
{
	t_philo	philosopher;

	philosopher.id = id;
	philosopher.rules = rules;
	philosopher.meals_eaten = 0;
	philosopher.last_meal = time_now_ms();
	while (1)
	{
		eat(&philosopher);
		philo_sleep(&philosopher);
		think(&philosopher);
	}
}

void	think(t_philo *philosopher)
{
	long long	start;

	start = philosopher->rules->start_time;
	if (i_am_alive(philosopher))
		printf("%lld %i is thinking\n", time_now_ms() - start, philosopher->id);
	else
		exit_death(philosopher);
}
void	philo_sleep(t_philo *philosopher)
{
	long long	start;

	start = philosopher->rules->start_time;
	if (i_am_alive(philosopher))
	{

		printf("%lld %i is sleeping\n", time_now_ms() - start, philosopher->id);
		sleep_and_aware(philosopher, philosopher->rules->time_to_sleep);
	}
	else
		exit_death(philosopher);
}

void	exit_death(t_philo *philosopher)
{
	sem_close(philosopher->rules->forks);
	exit(philosopher->id);
}

void	eat(t_philo *philosopher)
{
	if (i_am_alive(philosopher))
	{
		take_forks(philosopher);
		if (i_am_alive(philosopher))
		{
	
			philosopher->last_meal = time_now_ms();
			printf("%lld %i is eating\n", philosopher->last_meal
				- philosopher->rules->start_time, philosopher->id);
			sleep_and_aware(philosopher, philosopher->rules->time_to_eat);
			philosopher->meals_eaten++;
		}
		place_forks(philosopher);
		if (philosopher->meals_eaten == philosopher->rules->number_of_times_each_philosopher_must_eat)
			exit(0);
	}
	else
		exit_death(philosopher);
}

void	place_forks(t_philo *philosopher)
{
	sem_post(philosopher->rules->forks);
	sem_post(philosopher->rules->forks);
}

void	take_forks(t_philo *philosopher)
{
	int	id;

	id = philosopher->id;
	if (i_am_alive(philosopher) == false)
		exit_death(philosopher);
	sem_wait(philosopher->rules->forks);
	if (i_am_alive(philosopher))
		printf("%lld %i has taken a fork\n", time_now_ms()
			- philosopher->rules->start_time, id);
	else
	{
		sem_post(philosopher->rules->forks);
		exit_death(philosopher);
	}
	sem_wait(philosopher->rules->forks);
	if (i_am_alive(philosopher))
		printf("%lld %i has taken a fork\n", time_now_ms()
			- philosopher->rules->start_time, id);
	else
	{
		sem_post(philosopher->rules->forks);
		exit_death(philosopher);
	}
}

bool	i_am_alive(t_philo *philosopher)
{
	return (time_now_ms()
		- philosopher->last_meal <= philosopher->rules->time_to_die);
}

void	sleep_and_aware(t_philo *philosopher, long long milliseconds)
{
	long	start;

	start = time_now_ms();
	while ((time_now_ms() - start) < milliseconds)
	{
		if (i_am_alive(philosopher) == false)
			exit_death(philosopher);
		usleep(500);
	}
}

long long	time_now_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000LL + tv.tv_usec / 1000LL);
}
