/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 12:20:53 by fpedroso          #+#    #+#             */
/*   Updated: 2025/08/04 12:20:53 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*watchdog(void *philos)
{
	t_philo	*philosopher;
	int		i;

	if (!philos)
		return (NULL);
	philosopher = (t_philo *)philos;
	while (!simulation_has_started(philosopher->rules))
		usleep(500);
	sleep_millisecs(5);
	while (1)
	{
		i = -1;
		while (++i < philosopher->rules->number_of_philosophers)
		{
			if (is_dead(philosopher))
			{
				sleep_millisecs(3);
				atomic_print("died", philosopher);
				return (NULL);
			}
			philosopher = philosopher->right_fork->right_philo;
		}
		sleep_millisecs(5);
	}
	return (NULL);
}

bool	is_dead(t_philo *philosopher)
{
	long long	last_meal;

	pthread_mutex_lock(&philosopher->eat_mutex);
	last_meal = philosopher->last_meal;
	pthread_mutex_unlock(&philosopher->eat_mutex);
	if ((time_now_ms()
			- last_meal) > (long long)philosopher->rules->time_to_die)
	{
		pthread_mutex_lock(&philosopher->rules->death_mutex);
		philosopher->rules->someone_died = true;
		pthread_mutex_unlock(&philosopher->rules->death_mutex);
		return (true);
	}
	return (false);
}
