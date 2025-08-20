/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   general_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 15:14:54 by fpedroso          #+#    #+#             */
/*   Updated: 2025/08/20 23:43:32 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	exit_death(t_philo *philosopher, int status)
{
	sem_close(philosopher->rules->forks);
	sem_close(philosopher->rules->print_semaphore);
	exit(status);
}

void	sleep_millisecs(long long milliseconds)
{
	long	start;

	start = time_now_ms();
	while ((time_now_ms() - start) < milliseconds)
		usleep(500);
}

long long	time_now_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000LL + tv.tv_usec / 1000LL);
}
