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
		return (one_philo());
	gettimeofday(&philosopher->last_meal, NULL);
	while (1)
	{
		think();
		if (ate_enough(philosopher) || philosopher->rules->someone_died
			|| is_dead(philosopher))
			return (NULL);
		if (philosopher->id % 2 == 0)
		{
			pthread_mutex_lock(philosopher->right_fork->mutex);
			pthread_mutex_lock(philosopher->left_fork->mutex);
		}
		else
		{
			pthread_mutex_lock(philosopher->left_fork->mutex);
			pthread_mutex_lock(philosopher->right_fork->mutex);
		}
		eat(philosopher); // also logs the time of meal when starts (last_meal)
		pthread_mutex_unlock(philosopher->right_fork->mutex);
		pthread_mutex_unlock(philosopher->left_fork->mutex);
		philo_sleep(philosopher->rules->time_to_sleep);
	}
	return (NULL);
}


void	*life_monitor(philos)
{
	/*
		* Prints message when someone is dead
		* Sets termination flag to true accordingly
		* Terminates
		*/
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

void	make_philos(amount)
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
