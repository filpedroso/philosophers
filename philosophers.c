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
	if (pthread_mutex_init(rules.death_mutex, NULL) != 0)
		return (1);
	philos = prep_table(rules.number_of_philosophers, &rules);
	if (!philos)
	{
		terminate_simulation(philos);
		return (1);
	}
	status = start_simulation(philos);
	terminate_simulation(philos);
	return (status);
}

t_philo	*prep_table(int amount, t_rules *rules)
{
	t_philo	*new_philo;
	t_philo	*philos;
	int		id;

	if (amount <= 0 || !philos)
		return (NULL);
	philos = NULL;
	id = 0;
	while (id < amount)
	{
		new_philo = new_head_and_fork(id, rules);
		if (!new_philo)
			return (NULL);
		cdll_add(&philos, new_philo);
		id++;
	}
	return (philos);
}

t_philo	*new_head_and_fork(int id, t_rules *rules)
{
	t_philo		*philosopher;

	philosopher = (t_philo *)malloc(sizeof(t_philo));
	if (!philosopher)
		return (NULL);
	if (pthread_mutex_init(philosopher->right_fork->mutex, NULL) != 0)
	{
		free(philosopher);
		return (NULL);
	}
	philosopher->right_fork->left_philo = philosopher;
	philosopher->right_fork->right_philo = NULL;
	philosopher->left_fork = NULL;
	return (philosopher);
}

t_rules	parse_args(char **argv)
{
	t_rules	rules;

	rules.number_of_philosophers = ft_atoi(argv[1]);
	rules.time_to_die = ft_atoi(argv[2]);
	rules.time_to_eat = ft_atoi(argv[3]);
	rules.time_to_sleep = ft_atoi(argv[4]);
	rules.number_of_times_each_philosopher_must_eat = ft_atoi(argv[5]);
	rules.someone_died = false;
	rules.start_time = 0;
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
			return (ft_putstr_error("pthread create error\n"));
		head = head->right_fork->right_philo;
	}
	head = philos;
	n_philos = philos->rules->number_of_philosophers;
	while (n_philos--)
	{
		if (pthread_join(head->thread, NULL) != 0)
			return (ft_putstr_error("join error\n"));
		head = head->right_fork->right_philo;
	}
	if (pthread_create(&philos->rules->monitor_thread, NULL, watchdog, (void *)philos) != 0)
		return (ft_putstr_error("pthread create error\n"));
	return (pthread_detach(philos->rules->monitor_thread));
}

void	*watchdog(t_philo *philos)
{
	t_philo		*philosopher;

	philosopher = philos;
	while (is_dead(philosopher) == false)
	{
		philosopher = philosopher->right_fork->right_philo;
		usleep(50);
	}
	return (NULL);
}

int	ft_putstr_error(char *s)
{
	size_t	buffer;
	char	*ptr;

	if (!s)
		return (1);
	ptr = s;
	buffer = 0;
	while (*s)
	{
		buffer++;
		s++;
	}
	write(2, ptr, buffer);
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
	bool	should_stop;

	pthread_mutex_lock(philosopher->rules->death_mutex);
	should_stop = (ate_enough(philosopher) || philosopher->rules->someone_died);
	pthread_mutex_unlock(philosopher->rules->death_mutex);

}

bool	ate_enough(t_philo *philosopher)
{
	return (philosopher->meals_eaten
		>= philosopher->rules->number_of_times_each_philosopher_must_eat);
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
	philosopher->meals_eaten++;
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

bool	is_dead(t_philo *philosopher)
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
