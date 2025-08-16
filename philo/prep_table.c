/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prep_table.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 14:21:48 by fpedroso          #+#    #+#             */
/*   Updated: 2025/08/16 14:21:48 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_philo	*prep_table(int amount, t_rules *rules)
{
	t_philo	*new_philo;
	t_philo	*philos;
	int		id;

	if (amount <= 0)
		return (NULL);
	id = 0;
	philos = NULL;
	while (++id <= amount)
	{
		new_philo = new_philo_fork_pair(id);
		if (!new_philo)
		{
			terminate_simulation(philos);
			return (NULL);
		}
		new_philo->rules = rules;
		cdll_add(&philos, new_philo);
	}
	return (philos);
}

t_philo	*new_philo_fork_pair(int id)
{
	t_philo	*philosopher;

	philosopher = (t_philo *)malloc(sizeof(t_philo));
	if (!philosopher)
		return (NULL);
	memset(philosopher, 0, sizeof(t_philo));
	philosopher->right_fork = (t_fork *)malloc(sizeof(t_fork));
	if (!philosopher->right_fork)
		return (NULL);
	memset(philosopher->right_fork, 0, sizeof(t_fork));
	if (pthread_mutex_init(&philosopher->right_fork->mutex, NULL) != 0
		|| pthread_mutex_init(&philosopher->eat_mutex, NULL) != 0)
	{
		free(philosopher);
		return (NULL);
	}
	philosopher->id = id;
	philosopher->right_fork->id = id;
	philosopher->right_fork->left_philo = philosopher;
	philosopher->right_fork->being_used = false;
	return (philosopher);
}

void	cdll_add(t_philo **ptr, t_philo *new)
{
	if (!*ptr)
	{
		*ptr = new;
		new->left_fork = new->right_fork;
		new->right_fork->right_philo = new;
		return ;
	}
	new->right_fork->right_philo = (*ptr);
	new->left_fork = (*ptr)->left_fork;
	(*ptr)->left_fork->right_philo = new;
	(*ptr)->left_fork = new->right_fork;
}
