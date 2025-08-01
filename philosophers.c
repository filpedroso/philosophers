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

static void	start_simulation(t_philo *philos);

int	main(int argc, char **argv)
{
	t_rules	rules;
	t_philo	*philos;

	if (argc != 3)
		return (1);
	check_args(argc, argv); // exits cleanly if not right args
	rules = parse_args(argv);
	philos = make_philos(rules.number_of_philosophers);
	if (!philos)
		return (1);
	start_simulation(philos);
	terminate_simulation(philos);
	return (0);
}

static void	start_simulation(t_philo *philos)
{
	t_philo		*head;
	pthread_t	monitor;
	int			n_philos;

	n_philos = philos->rules->number_of_philosophers;
	head = philos;
	while (n_philos--)
	{
		pthread_create(&head->thread, NULL, routine, (void *)head);
		head = head->right_fork->right_philo;
	}
	pthread_create(&monitor, NULL, life_monitor, (void *)philos);
	head = philos;
	n_philos = philos->rules->number_of_philosophers;
	while (n_philos--)
	{
		pthread_join(head->thread, NULL);
		head = head->right_fork->right_philo;
	}
	pthread_detach(monitor);
}

void	*routine(t_philo *philosopher)
{
	if (philosopher->rules->number_of_philosophers == 1)
	{
		one_philo();
		return (NULL);
	}
	gettimeofday(&philosopher->last_meal, NULL);
	while (1)
	{
		if (should_stop(philosopher))
		{
			return (NULL);
		}
		think();
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

	now = get_time_ms();
	printf("%lld %i is eating\n", now, philosopher->id);
	philosopher->last_meal = now;
	usleep(philosopher->rules->time_to_eat * 1000);
}

long long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000LL + tv.tv_usec / 1000);
}

void	place_forks(t_philo *philosopher)
{
	pthread_mutex_unlock(philosopher->right_fork->mutex);
	pthread_mutex_unlock(philosopher->left_fork->mutex);
}

void	take_forks(t_philo *philosopher)
{
	int	philosopher_id;

	philosopher_id = philosopher->id;
	if (philosopher_id % 2 == 0)
	{
		pthread_mutex_lock(philosopher->right_fork->mutex);
		printf("%i has taken a fork\n", philosopher_id);
		pthread_mutex_lock(philosopher->left_fork->mutex);
		printf("%i has taken a fork\n", philosopher_id);
	}
	else
	{
		pthread_mutex_lock(philosopher->left_fork->mutex);
		printf("%i has taken a fork\n", philosopher_id);
		pthread_mutex_lock(philosopher->right_fork->mutex);
		printf("%i has taken a fork\n", philosopher_id);
	}
}

// returns true if is dead, but also prints the log and updates someone_died
int	is_dead(t_philo *philosopher)
{
	long long	now;

	now = get_time_ms();
	if (now - philosopher->last_meal > philosopher->rules->time_to_die)
	{
		printf("%i died\n", philosopher->id);
		philosopher->rules->someone_died = true;
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

void	parse_args(char **args)
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
