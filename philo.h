/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 13:59:49 by fpedroso          #+#    #+#             */
/*   Updated: 2025/07/30 13:59:49 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <string.h>
# include <stdbool.h>
# include <unistd.h>

typedef struct s_fork;

typedef struct s_rules
{
	int				number_of_philosophers;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				number_of_times_each_philosopher_must_eat;
	int				someone_died;
	long long		start_time;
}					t_rules;

typedef struct s_philo
{
	int				id;
	bool			is_dead;
	pthread_t		thread;
	struct timeval	last_meal;
	int				meals_eaten;
	t_rules			*rules;
	struct s_fork	*left_fork;
	struct s_fork	*right_fork;
	pthread_mutex_t	*death_mutex;
	pthread_mutex_t	*state_mutex;
}					t_philo;

typedef struct s_fork
{
	int				id;
	pthread_mutex_t	*mutex;
	t_philo			*left_philo;
	t_philo			*right_philo;
}					t_fork;


void	*routine(t_philo *philosopher);
void	*life_monitor(t_philo * philos);
void	check_args(int argc, char **argv);
void	make_philos(int amount);
void	terminate_simulation(t_philo *philos);
void	parse_args(char **args);

#endif
