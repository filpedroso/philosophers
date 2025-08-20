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

void take_forks(t_philo *philosopher)
{
    pthread_mutex_t first;
    pthread_mutex_t second;

	get_mutex_order(&first, &second, philosopher);
    while (1)
    {
        pthread_mutex_lock(&first);
        pthread_mutex_lock(&second);
        if (!philosopher->left_fork->being_used && !philosopher->right_fork->being_used)
        {
            if (!should_stop(philosopher))
            {
				philosopher->left_fork->being_used = true;
				philosopher->right_fork->being_used = true;
				atomic_print("has taken a fork", philosopher);
				atomic_print("has taken a fork", philosopher);
            }
			pthread_mutex_unlock(&first);
			pthread_mutex_unlock(&second);
			return;
        }
        pthread_mutex_unlock(&first);
        pthread_mutex_unlock(&second);
        usleep(100);
    }
}

void	get_mutex_order(pthread_mutex_t *first, pthread_mutex_t *second,
	t_philo *philosopher)
{
    if (philosopher->left_fork->id < philosopher->right_fork->id)
    {
        *first = philosopher->left_fork->mutex;
        *second = philosopher->right_fork->mutex;
    }
    else
    {
        *first = philosopher->right_fork->mutex;
        *second = philosopher->left_fork->mutex;
    }
}

/* void	take_forks(t_philo *philosopher)
{
	long long	start;

	start = philosopher->rules->start_time;
	while (i_am_alive(philosopher))
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
} */

void	place_forks(t_philo *philosopher)
{
	pthread_mutex_lock(&philosopher->left_fork->mutex);
	philosopher->left_fork->being_used = false;
	pthread_mutex_unlock(&philosopher->left_fork->mutex);
	pthread_mutex_lock(&philosopher->right_fork->mutex);
	philosopher->right_fork->being_used = false;
	pthread_mutex_unlock(&philosopher->right_fork->mutex);
}
