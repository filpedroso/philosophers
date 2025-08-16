/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_simulation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 14:24:46 by fpedroso          #+#    #+#             */
/*   Updated: 2025/08/16 14:24:46 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	start_simulation(t_philo *philos)
{
	int	n_philos;

	n_philos = philos->rules->number_of_philosophers;
	if (!create_threads(philos, n_philos))
		return (1);
	if (!create_monitor_detached(philos))
		return (1);
	if (pthread_mutex_init(&philos->rules->simul_mutex, NULL) != 0)
		return (false);
	if (pthread_mutex_lock(&philos->rules->simul_mutex) != 0)
		return (false);
	philos->rules->simulation_started = true;
	philos->rules->start_time = time_now_ms();
	pthread_mutex_unlock(&philos->rules->simul_mutex);
	if (!join_philos(philos, n_philos))
		return (1);
	return (0);
}

bool	create_monitor_detached(t_philo *philos)
{
	if (pthread_create(&philos->rules->monitor_thread, NULL, watchdog,
			(void *)philos) != 0)
	{
		ft_putstr_error("pthread monitor create error\n");
		return (false);
	}
	if (pthread_detach(philos->rules->monitor_thread) != 0)
	{
		ft_putstr_error("pthread monitor detach error\n");
		return (false);
	}
	return (true);
}

bool	create_threads(t_philo *philos, int n_philos)
{
	t_philo	*head;

	head = philos;
	while (n_philos--)
	{
		if (pthread_create(&head->thread, NULL, routine, (void *)head) != 0)
		{
			ft_putstr_error("pthread philo create error\n");
			return (false);
		}
		head = head->right_fork->right_philo;
	}
	return (true);
}

bool	join_philos(t_philo *philos, int n_philos)
{
	t_philo	*head;

	head = philos;
	while (n_philos--)
	{
		if (pthread_join(head->thread, NULL) != 0)
			return (ft_putstr_error("join error\n"));
		head = head->right_fork->right_philo;
	}
	return (true);
}
