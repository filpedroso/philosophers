/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 16:43:40 by fpedroso          #+#    #+#             */
/*   Updated: 2025/08/17 16:43:40 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	t_rules	rules;
	t_philo	*philos;
	int		status;

	if ((argc != 5) && (argc != 6))
		return (1);
	if (!parse_args(argc, argv, &rules))
		return (1);
	philos = prep_table(rules.number_of_philosophers, &rules);
	if (!philos)
		return (1);
	status = 0;
	status = start_simulation(philos);
	terminate_simulation(philos);
	return (status);
}

bool	parse_args(int argc, char **argv, t_rules *rules)
{
	memset((void *)rules, 0, sizeof(t_rules));
	rules->number_of_philosophers = atoi_positive(argv[1]);
	rules->time_to_die = atoi_positive(argv[2]);
	rules->time_to_eat = atoi_positive(argv[3]);
	rules->time_to_sleep = atoi_positive(argv[4]);
	if (argc == 6)
		rules->number_of_times_each_philosopher_must_eat
			= atoi_positive(argv[5]);
	else
		rules->number_of_times_each_philosopher_must_eat = NO_ARG;
	if (rules->number_of_philosophers == -1 || rules->time_to_die == -1
		|| rules->time_to_eat == -1 || rules->time_to_sleep == -1
		|| rules->number_of_times_each_philosopher_must_eat == -1)
		return (false);
	return (true);
}

int	atoi_positive(char *str)
{
	int	num;

	if (!str || !*str)
		return (-1);
	num = 0;
	while (*str)
	{
		if (*str < '0' || *str > '9')
			return (-1);
		num = num * 10 + (*str - '0');
		str++;
	}
	return (num);
}

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
		new_philo = new_philo_node(id);
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

t_philo	*new_philo_node(int id)
{
	t_philo	*philosopher;

	philosopher = (t_philo *)malloc(sizeof(t_philo));
	if (!philosopher)
		return (NULL);
	memset(philosopher, 0, sizeof(t_philo));
	philosopher->id = id;
	philosopher->left_philo = philosopher;
	philosopher->right_philo = philosopher;
	return (philosopher);
}

void	cdll_add(t_philo **ptr, t_philo *new)
{
	if (!*ptr)
	{
		*ptr = new;
		return ;
	}
	new->right_philo = (*ptr);
	new->left_philo = (*ptr)->left_philo;
	(*ptr)->left_philo->right_philo = new;
	(*ptr)->left_philo = new;
}
