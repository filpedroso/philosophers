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

int	main(int argc, char **argv)
{
	t_rules	rules;
	t_philo	*philos;
	int		status;

	if ((argc != 5 && argc != 6) || !parse_args(argc, argv, &rules))
	{
		printf("Error: invalid args\n");
		return (1);
	}
	if (!parse_args(argc, argv, &rules))
		return (1);
	if (pthread_mutex_init(&rules.death_mutex, NULL) != 0)
		return (1);
	if (pthread_mutex_init(&rules.print_mutex, NULL) != 0)
		return (1);
	philos = prep_table(rules.number_of_philosophers, &rules);
	if (!philos)
		return (1);
	status = 0;
	status = start_simulation(philos);
	terminate_simulation(philos);
	return (status);
}

void	one_philo(t_philo *philosopher)
{
	long long	now;
	long long	start;

	start = philosopher->rules->start_time;
	now = time_now_ms();
	pthread_mutex_lock(&philosopher->right_fork->mutex);
	philosopher->right_fork->being_used = true;
	printf("%lld %i has taken a fork\n", now - start, philosopher->id);
	pthread_mutex_unlock(&philosopher->right_fork->mutex);
	sleep_millisecs(philosopher->rules->time_to_die);
	pthread_mutex_lock(&philosopher->rules->death_mutex);
	philosopher->rules->someone_died = true;
	printf("%lld %i died\n", now - start, philosopher->id);
	pthread_mutex_unlock(&philosopher->rules->death_mutex);
	return ;
}

void	atomic_print(char *msg, t_philo *philosopher)
{
	pthread_mutex_lock(&philosopher->rules->print_mutex);
	printf("%lld %i %s\n",
		(time_now_ms() - philosopher->rules->start_time),
		philosopher->id, msg);
	pthread_mutex_unlock(&philosopher->rules->print_mutex);
}

/* void	debug_print_myself(t_philo *philos)
{
	printf("philo id: %i\n", philos->id);
	printf("last meal: %lld\n", philos->last_meal);
	printf("left fork id: %i\n", philos->left_fork->id);
	printf("right fork id: %i\n", philos->right_fork->id);
	printf("meals eaten: %i\n", philos->meals_eaten);
	printf("rules:\n");
	debug_print_rules(*philos->rules);
	printf("      | \n");
	printf("      | \n");
	printf("      | \n");
	printf("      V \n");
}

void	debug_print_philos_list(t_philo *philos)
{
	for(int i = 1; i <= philos->rules->number_of_philosophers; i++) {
		printf("philo id: %i\n", philos->id);
		printf("last meal: %lld\n", philos->last_meal);
		printf("left fork id: %i\n", philos->left_fork->id);
		printf("right fork id: %i\n", philos->right_fork->id);
		printf("meals eaten: %i\n", philos->meals_eaten);
		printf("rules:\n");
		debug_print_rules(*philos->rules);
		printf("      | \n");
		printf("      | \n");
		printf("      | \n");
		printf("      V \n");
		philos = philos->right_fork->right_philo;
	}
}

void	debug_print_rules(t_rules rules)
{
	printf("number of philos: %i\n", rules.number_of_philosophers);
	printf("number philos must eat: %i\n",
		rules.number_of_times_each_philosopher_must_eat);
	printf("someone died: %i\n", rules.someone_died);
	printf("start time: %lld\n", rules.start_time);
	printf("time to die: %i\n", rules.time_to_die);
	printf("time to eat: %i\n", rules.time_to_eat);
	printf("time to sleep: %i\n", rules.time_to_sleep);
} */
