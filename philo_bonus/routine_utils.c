/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 15:12:01 by fpedroso          #+#    #+#             */
/*   Updated: 2025/08/20 23:54:07 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	take_forks(t_philo *philosopher)
{
	if (i_am_alive(philosopher) == false)
		exit_death(philosopher, philosopher->id);
	sem_wait(philosopher->rules->forks);
	sem_wait(philosopher->rules->forks);
	if (i_am_alive(philosopher))
		atomic_print("has taken a fork", philosopher);
	else
	{
		sem_post(philosopher->rules->forks);
		exit_death(philosopher, philosopher->id);
	}
	if (i_am_alive(philosopher))
		atomic_print("has taken a fork", philosopher);
	else
	{
		sem_post(philosopher->rules->forks);
		exit_death(philosopher, philosopher->id);
	}
}

void	place_forks(t_philo *philosopher)
{
	sem_post(philosopher->rules->forks);
	sem_post(philosopher->rules->forks);
}

void	sleep_and_aware(t_philo *philosopher, long long milliseconds)
{
	long	start;

	start = time_now_ms();
	while ((time_now_ms() - start) < milliseconds)
	{
		if (i_am_alive(philosopher) == false)
			exit_death(philosopher, philosopher->id);
		usleep(500);
	}
}

bool	i_am_alive(t_philo *philosopher)
{
	return (time_now_ms()
		- philosopher->last_meal <= philosopher->rules->time_to_die);
}

bool	is_starving(t_philo *philosopher)
{
	int	base_time_left;
	int	base_time_spent;

	base_time_spent = philosopher->rules->time_to_eat
		+ philosopher->rules->time_to_sleep;
	base_time_left = philosopher->rules->time_to_die - base_time_spent;
	return ((time_now_ms() - philosopher->last_meal) > (base_time_spent
			+ base_time_left / 10));
}
