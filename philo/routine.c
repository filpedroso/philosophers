/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 14:31:05 by fpedroso          #+#    #+#             */
/*   Updated: 2025/08/16 14:31:05 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*routine(void *arg)
{
	t_philo	*philosopher;

	philosopher = (t_philo *)arg;
	if (!philosopher)
		return (NULL);
	while (!simulation_has_started(philosopher->rules))
		usleep(500);
	pthread_mutex_lock(&philosopher->eat_mutex);
	philosopher->last_meal = time_now_ms();
	pthread_mutex_unlock(&philosopher->eat_mutex);
	if (philosopher->rules->number_of_philosophers == 1)
		return (one_philo(philosopher), NULL);
	while (1)
	{
		if (!eat(philosopher))
			return (NULL);
		if (!philo_sleep(philosopher))
			return (NULL);
		if (!think(philosopher))
			return (NULL);
	}
	return (NULL);
}

bool	eat(t_philo *philosopher)
{
	if (i_am_alive(philosopher) && !should_stop(philosopher))
	{
		take_forks(philosopher);
		pthread_mutex_lock(&philosopher->eat_mutex);
		philosopher->last_meal = time_now_ms();
		pthread_mutex_unlock(&philosopher->eat_mutex);
		if (should_stop(philosopher))
			return (false);
		printf("%lld %i is eating\n", philosopher->last_meal
			- philosopher->rules->start_time, philosopher->id);
		sleep_millisecs(philosopher->rules->time_to_eat);
		place_forks(philosopher);
		pthread_mutex_lock(&philosopher->eat_mutex);
		philosopher->meals_eaten++;
		pthread_mutex_unlock(&philosopher->eat_mutex);
	}
	else
		return (false);
	return (true);
}

bool	philo_sleep(t_philo *philosopher)
{
	long long	start;

	if (!philosopher)
		return (false);
	if (should_stop(philosopher))
		return (false);
	start = philosopher->rules->start_time;
	printf("%lld %i is sleeping\n", time_now_ms() - start, philosopher->id);
	sleep_millisecs(philosopher->rules->time_to_sleep);
	return (true);
}

bool	think(t_philo *philosopher)
{
	long long	now;
	long long	start;
	int			id;

	if (!philosopher)
		return (false);
	if (should_stop(philosopher))
		return (false);
	start = philosopher->rules->start_time;
	now = time_now_ms();
	id = philosopher->id;
	printf("%lld %i is thinking\n", now - start, id);
	while (!is_starving(philosopher))
		sleep_millisecs(5);
	return (true);
}
