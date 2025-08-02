/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 15:45:58 by fpedroso          #+#    #+#             */
/*   Updated: 2025/07/30 16:35:50 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	start_simulation(t_philo *philos);

int	main(int argc, char **argv)
{
	t_rules	rules;
	t_philo	*philos;
	int		status;

	if (argc != 3)
		return (1);
	status = 0;
	check_args(argc, argv); // exits cleanly if not right args
	rules = parse_args(argv);
	philos = make_philos(rules.number_of_philosophers);
	if (!philos)
		return (1);
	status = start_simulation(philos);
	terminate_simulation(philos);
	return (status);
}

static int	start_simulation(t_philo *philos)
{
	t_philo		*head;
	int			n_philos;

	n_philos = philos->rules->number_of_philosophers;
	head = philos;
	philos->rules->start_time = time_now_ms();
	while (n_philos--)
	{
		if (pthread_create(&head->thread, NULL, routine, (void *)head) != 0)
			return (ft_putstr_fd("pthread create error\n", 2));
		head = head->right_fork->right_philo;
	}
	head = philos;
	n_philos = philos->rules->number_of_philosophers;
	while (n_philos--)
	{
		if (pthread_join(head->thread, NULL) != 0)
			return (ft_putstr_fd("join error\n", 2));
		head = head->right_fork->right_philo;
	}
	return (0);
}

int	ft_putstr_fd(char *s, int fd)
{
	size_t	buffer;

	if (!s)
		return (1);
	buffer = 0;
	while (*s)
	{
		buffer++;
		s++;
	}
	write(fd, s, buffer);
	return (1);
}

void	*routine(t_philo *philosopher)
{
	philosopher->last_meal = time_now_ms();
	if (philosopher->rules->number_of_philosophers == 1)
	{
		one_philo(philosopher);
		return (NULL);
	}
	while (1)
	{
		if (should_stop(philosopher))
		{
			return (NULL);
		}
		think(philosopher);
		take_forks(philosopher);
		eat(philosopher);
		place_forks(philosopher);
		if (should_stop(philosopher))
		{
			return (NULL);
		}
		philo_sleep(philosopher);
	}
	return (NULL);
}

void	think(t_philo *philosopher)
{
	long long	now;
	long long	start;

	start = philosopher->rules->start_time;
	now = time_now_ms();
	printf("%lld %i is thinking\n", now - start, philosopher->id);
}

void	one_philo(t_philo *philosopher)
{
	long long	now;
	long long	start;

	start = philosopher->rules->start_time;
	now = time_now_ms();
	pthread_mutex_lock(philosopher->right_fork->mutex);
	printf("%lld %i has taken a fork\n", now - start, philosopher->id);
	usleep(philosopher->rules->time_to_die * 1000);
	is_dead(philosopher);
	pthread_mutex_unlock(philosopher->right_fork->mutex);
	return ;
}

bool	should_stop(t_philo *philosopher)
{
	return (ate_enough(philosopher) || philosopher->rules->someone_died
		|| is_dead(philosopher));
}

bool	ate_enough(t_philo *philosopher)
{
	return (philosopher->meals_eaten >= philosopher->rules->number_of_times_each_philosopher_must_eat);
}

void	eat(t_philo *philosopher)
{
	long long	now;
	long long	start;

	start = philosopher->rules->start_time;
	now = time_now_ms();
	printf("%lld %i is eating\n", now - start, philosopher->id);
	philosopher->last_meal = now;
	usleep(philosopher->rules->time_to_eat * 1000);
}

long long	time_now_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000LL + tv.tv_usec / 1000);
}

void	place_forks(t_philo *philosopher)
{
	int	philosopher_id;

	philosopher_id = philosopher->id;
	if (philosopher_id % 2 == 0)
	{
		pthread_mutex_unlock(philosopher->left_fork->mutex);
		pthread_mutex_unlock(philosopher->right_fork->mutex);

	}
	else
	{
		pthread_mutex_unlock(philosopher->right_fork->mutex);
		pthread_mutex_unlock(philosopher->left_fork->mutex);
	}
}

void	take_forks(t_philo *philosopher)
{
	int			philosopher_id;
	long long	now;
	long long	start;

	start = philosopher->rules->start_time;
	philosopher_id = philosopher->id;
	if (philosopher_id % 2 == 0)
	{
		pthread_mutex_lock(philosopher->right_fork->mutex);
		now = time_now_ms();
		printf("%lld %i has taken a fork\n", now - start, philosopher_id);
		pthread_mutex_lock(philosopher->left_fork->mutex);
		now = time_now_ms();
		printf("%lld %i has taken a fork\n", now - start, philosopher_id);
	}
	else
	{
		pthread_mutex_lock(philosopher->left_fork->mutex);
		now = time_now_ms();
		printf("%lld %i has taken a fork\n", now - start, philosopher_id);
		pthread_mutex_lock(philosopher->right_fork->mutex);
		now = time_now_ms();
		printf("%lld %i has taken a fork\n", now - start, philosopher_id);
	}
}

int	is_dead(t_philo *philosopher)
{
	long long	now;
	long long	start;

	start = philosopher->rules->start_time;
	now = time_now_ms();
	if (now - philosopher->last_meal > philosopher->rules->time_to_die)
	{
		pthread_mutex_lock(philosopher->rules->death_mutex);
		if (philosopher->rules->someone_died == false)
		{
			philosopher->rules->someone_died = true;
			printf("%lld %i died\n", now - start, philosopher->id);
		}
		pthread_mutex_unlock(philosopher->rules->death_mutex);
		return (true);
	}
	return (false);
}

void	check_args(int argc, char **argv)
{
	/*
		* Checks args validity per exercise sheet
		* Look for correction sheet online
		* Exit with (1) if not right args
		*/
}

t_rules	parse_args(char **args)
{
	/*
		* Fill the struct with a simple atouint on each arg
		*/
}

t_philo	*make_philos(amount)
{
	/*
		* Makes philos and forks, the same amount each
		* Circular doubly linked list, where
		* each philo is a thread and has forks each side
		* Each fork is a mutex and also a node in the DLL
		*/
}

void	terminate_simulation(t_philo *philos)
{
	/*
		* Destroy mutexes and free structs
		*/
}

/* UNUSED - OBSOLETE
void	*life_monitor(t_philo *philos)
{
	t_philo	*head;

	head = philos;
	while (1)
	{
	}
	/*
		* Prints message when someone is dead
		* Sets termination flag to true accordingly
		* Terminates
}
*/
