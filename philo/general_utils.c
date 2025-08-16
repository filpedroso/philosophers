/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   general_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 14:37:24 by fpedroso          #+#    #+#             */
/*   Updated: 2025/08/16 14:37:24 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

void	terminate_simulation(t_philo *philos)
{
	t_philo	*philo_ptr;
	t_fork	*fork_ptr;
	int		n_philos;
	int		i;

	if (!philos)
		return ;
	n_philos = philos->left_fork->left_philo->id;
	i = 0;
	pthread_mutex_destroy(&philos->rules->death_mutex);
	pthread_mutex_destroy(&philos->rules->simul_mutex);
	while (i < n_philos)
	{
		philo_ptr = philos;
		fork_ptr = philos->right_fork;
		pthread_mutex_destroy(&fork_ptr->mutex);
		pthread_mutex_destroy(&philo_ptr->eat_mutex);
		philos = philos->right_fork->right_philo;
		free(philo_ptr);
		free(fork_ptr);
		i++;
	}
}

int	ft_putstr_error(char *s)
{
	size_t	buffer;
	char	*ptr;

	if (!s)
		return (1);
	ptr = s;
	buffer = 0;
	while (*s)
	{
		buffer++;
		s++;
	}
	write(2, ptr, buffer);
	return (1);
}
