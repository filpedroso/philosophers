/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 13:59:49 by fpedroso          #+#    #+#             */
/*   Updated: 2025/08/20 23:53:27 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <fcntl.h>
# include <limits.h>
# include <pthread.h>
# include <semaphore.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/time.h>
# include <sys/wait.h>
# include <unistd.h>

# define NO_ARG -2

typedef struct s_rules
{
	int			number_of_philosophers;
	int			time_to_die;
	int			time_to_eat;
	int			time_to_sleep;
	int			number_of_times_each_philosopher_must_eat;
	long long	start_time;
	sem_t		*forks;
	sem_t		*print_semaphore;
}				t_rules;

typedef struct s_philo
{
	int			id;
	long long	last_meal;
	int			meals_eaten;
	t_rules		*rules;
}				t_philo;

void			create_processes(t_rules *rules, pid_t *pids);
bool			parse_args(int argc, char **argv, t_rules *rules);
int				atoi_positive(char *str);
int				simulation(t_rules rules);
void			routine(t_rules *rules, int id);
void			think(t_philo *philosopher);
void			one_philo(t_philo *philosopher);
void			eat(t_philo *philosopher);
long long		time_now_ms(void);
void			place_forks(t_philo *philosopher);
void			take_forks(t_philo *philosopher);
void			reap_processes(t_rules *rules, pid_t *pids);
void			philo_sleep(t_philo *philosopher);
void			sleep_and_aware(t_philo *philosopher, long long milliseconds);
bool			i_am_alive(t_philo *philosopher);
void			exit_death(t_philo *philosopher, int status);
void			sleep_millisecs(long long milliseconds);
bool			is_starving(t_philo *philosopher);
void			one_philo(t_philo *philosopher);
void			atomic_print(char *msg, t_philo *philosopher);
void			unlink_and_close_semaphores(t_rules *rules);

#endif
