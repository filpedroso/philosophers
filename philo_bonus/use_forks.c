/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   use_forks.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 14:33:55 by fpedroso          #+#    #+#             */
/*   Updated: 2025/08/16 14:33:55 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	take_forks(t_philo *philosopher)
{
	long long	start;

	start = philosopher->rules->start_time;
	while (1)
	{
		pthread_mutex_lock(&philosopher->left_fork->mutex);
		pthread_mutex_lock(&philosopher->right_fork->mutex);
		if (!philosopher->left_fork->being_used
			&& !philosopher->right_fork->being_used)
		{
			philosopher->left_fork->being_used = true;
			printf("%lld %i has taken a fork\n", time_now_ms()
				- start, philosopher->id);
			philosopher->right_fork->being_used = true;
			printf("%lld %i has taken a fork\n", time_now_ms()
				- start, philosopher->id);
			pthread_mutex_unlock(&philosopher->left_fork->mutex);
			pthread_mutex_unlock(&philosopher->right_fork->mutex);
			break ;
		}
		pthread_mutex_unlock(&philosopher->left_fork->mutex);
		pthread_mutex_unlock(&philosopher->right_fork->mutex);
		usleep(100);
	}
}

void	place_forks(t_philo *philosopher)
{
	pthread_mutex_lock(&philosopher->left_fork->mutex);
	philosopher->left_fork->being_used = false;
	pthread_mutex_unlock(&philosopher->left_fork->mutex);
	pthread_mutex_lock(&philosopher->right_fork->mutex);
	philosopher->right_fork->being_used = false;
	pthread_mutex_unlock(&philosopher->right_fork->mutex);
}
