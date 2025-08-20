/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 15:09:27 by fpedroso          #+#    #+#             */
/*   Updated: 2025/08/20 23:47:53 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	routine(t_rules *rules, int id)
{
	t_philo	philosopher;

	philosopher.id = id;
	philosopher.rules = rules;
	philosopher.meals_eaten = 0;
	philosopher.last_meal = time_now_ms();
	if (rules->number_of_philosophers == 1)
		one_philo(&philosopher);
	while (1)
	{
		eat(&philosopher);
		philo_sleep(&philosopher);
		think(&philosopher);
	}
}

void	one_philo(t_philo *philosopher)
{
	sem_wait(philosopher->rules->forks);
	atomic_print("has taken a fork", philosopher);
	sleep_millisecs(philosopher->rules->time_to_die);
	sem_close(philosopher->rules->forks);
	exit_death(philosopher, philosopher->id);
}

void	eat(t_philo *philosopher)
{
	if (i_am_alive(philosopher))
	{
		take_forks(philosopher);
		if (i_am_alive(philosopher))
		{
			philosopher->last_meal = time_now_ms();
			atomic_print("is eating", philosopher);
			sleep_millisecs((long long)philosopher->rules->time_to_eat);
			philosopher->meals_eaten++;
		}
		place_forks(philosopher);
		if (philosopher->meals_eaten
			== philosopher->rules->number_of_times_each_philosopher_must_eat)
			exit_death(philosopher, 0);
	}
	else
		exit_death(philosopher, philosopher->id);
}

void	philo_sleep(t_philo *philosopher)
{
	if (i_am_alive(philosopher))
	{
		atomic_print("is sleeping", philosopher);
		sleep_and_aware(philosopher, philosopher->rules->time_to_sleep);
	}
	else
		exit_death(philosopher, philosopher->id);
}

void	think(t_philo *philosopher)
{
	if (i_am_alive(philosopher))
	{
		atomic_print("is thinking", philosopher);
		while (!is_starving(philosopher))
			sleep_millisecs(1);
	}
	else
		exit_death(philosopher, philosopher->id);
}
