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

	if ((argc != 5 && argc != 6) || !parse_args(argc, argv, &rules))
	{
		printf("Invalid args\n");
		return (1);
	}
	sem_unlink("forks");
	status = 0;
	status = simulation(rules);
	return (status);
}

void	atomic_print(char *msg, t_philo *philosopher)
{
	sem_wait(philosopher->rules->print_semaphore);
	printf("%lld %i %s\n",
		(time_now_ms() - philosopher->rules->start_time),
		philosopher->id, msg);
	sem_post(philosopher->rules->print_semaphore);
}

