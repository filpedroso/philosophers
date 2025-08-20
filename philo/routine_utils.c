/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 14:35:40 by fpedroso          #+#    #+#             */
/*   Updated: 2025/08/16 14:35:40 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	ate_enough(t_philo *philosopher)
{
	int	meals_eaten;
	int	must_eat;

	must_eat = philosopher->rules->number_of_times_each_philosopher_must_eat;
	if (must_eat == NO_ARG)
		return (false);
	pthread_mutex_lock(&philosopher->eat_mutex);
	meals_eaten = philosopher->meals_eaten;
	pthread_mutex_unlock(&philosopher->eat_mutex);
	if (meals_eaten >= must_eat)
	{
		return (true);
	}
	return (false);
}

bool	is_starving(t_philo *philosopher)
{
	bool	is_starving;
	int		base_time_left;
	int		base_time_spent;

	base_time_spent = philosopher->rules->time_to_eat
		+ philosopher->rules->time_to_sleep;
	base_time_left = philosopher->rules->time_to_die - base_time_spent;
	pthread_mutex_lock(&philosopher->eat_mutex);
	is_starving = (time_now_ms() - philosopher->last_meal) > (base_time_spent
			+ base_time_left / 10);
	pthread_mutex_unlock(&philosopher->eat_mutex);
	return (is_starving);
}

bool	should_stop(t_philo *philosopher)
{
	bool	someone_died;

	someone_died = false;
	pthread_mutex_lock(&philosopher->rules->death_mutex);
	someone_died = philosopher->rules->someone_died;
	pthread_mutex_unlock(&philosopher->rules->death_mutex);
	if (someone_died)
		return (true);
	if (philosopher->rules->number_of_times_each_philosopher_must_eat != NO_ARG)
		return (ate_enough(philosopher));
	return (false);
}

bool	i_am_alive(t_philo *philosopher)
{
	return ((time_now_ms() - philosopher->last_meal)
		<= (long long)philosopher->rules->time_to_die);
}

bool	simulation_has_started(t_rules *rules)
{
	bool	has_started;

	if (pthread_mutex_lock(&rules->simul_mutex) != 0)
		return (false);
	has_started = rules->simulation_started;
	pthread_mutex_unlock(&rules->simul_mutex);
	return (has_started);
}
