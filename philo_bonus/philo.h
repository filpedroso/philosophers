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
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>
# include <semaphore.h>

# define NO_ARG -2

typedef struct s_fork	t_fork;

typedef struct s_rules
{
	int					number_of_philosophers;
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					number_of_times_each_philosopher_must_eat;
	long long			start_time;
	sem_t				*forks;
}						t_rules;

typedef struct s_philo
{
	int					id;
	long long			last_meal;
	int					meals_eaten;
	t_rules				rules;
}							t_philo;

typedef struct	s_forks
{
	...;
}				t_forks;

bool					parse_args(int argc, char **argv, t_rules *rules);
int						atoi_positive(char *str);
t_philo					*prep_table(int amount, t_rules *rules);
void					cdll_add(t_philo **ptr, t_philo *new);
t_philo					*new_philo_fork_pair(int id);
int						start_simulation(t_philo *philos);
void					*watchdog(void *philos);
int						ft_putstr_error(char *s);
void					*routine(void *arg);
bool					think(t_philo *philosopher);
void					one_philo(t_philo *philosopher);
bool					ate_enough(t_philo *philosopher);
bool					should_stop(t_philo *philosopher);
bool					eat(t_philo philosopher);
long long				time_now_ms(void);
void					place_forks(t_philo *philosopher);
void					take_forks(t_philo *philosopher);
bool					is_dead(t_philo *philosopher);
void					terminate_simulation(t_philo *philos);
bool					philo_sleep(t_philo *philosopher);
bool					join_philos(t_philo *philos, int n_philos);
bool					create_monitor_detached(t_philo *philos);
bool					create_threads(t_philo *philos, int n_philos);
void					sleep_millisecs(long long usec);
bool					simulation_has_started(t_rules *rules);
bool					i_am_alive(t_philo philosopher);
bool					is_starving(t_philo *philosopher);

#endif
