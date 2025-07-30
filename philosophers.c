/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 15:45:58 by fpedroso          #+#    #+#             */
/*   Updated: 2025/07/30 16:35:50 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int main(int argc, char **argv)
{
	t_rules	rules;
	t_philo	*philos;
	t_philo	*head;
	int		i;

	if (argc != 3)
		return (1);
	check_args(argc, argv); //exits cleanly if not right args
	rules = parse_args(argv);
	philos = make_philos(rules.n_philos);
	head = philos;
	i = -1;
	while (++i < rules.n_philos)
	{
		pthread_create(head->thread, NULL, routine, (void *)&head);
		head = head->right_fork->right_philo;
	}
	i = -1;
	while (++i < rules.n_philos)
	{
		pthread_join(head->thread); // can you do this? won't it halt the current thread in the first join?
		head = head->right_fork->right_philo;
	}
}

