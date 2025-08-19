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

#include "philo_bonus.h"

int	main(int argc, char **argv)
{
	t_rules	rules;
	int		status;

	if ((argc != 5) && (argc != 6))
		return (1);
	if (!parse_args(argc, argv, &rules))
		return (1);
	sem_unlink("forks");
	status = 0;
	status = simulation(rules);
	return (status);
}
