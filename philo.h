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
# include <unistd.h>

typedef struct s_fork;

typedef struct s_rules
{
	int				n_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				must_eat;
	int				someone_died;
	long long		start_time;
}					t_rules;

typedef struct s_philo
{
	int				id;
	pthread_t		thread;
	long long		last_meal;
	int				meals_eaten;
	struct s_fork	*left_fork;
	struct s_fork	*right_fork;
	t_rules			*rules;
}					t_philo;

typedef struct s_fork
{
	int				id;
	pthread_mutex_t	mutex;
	t_philo			*left_philo;
	t_philo			*right_philo;
}					t_fork;



/* init.c */
int					init_all(t_rules *rules, int argc, char **argv);

/* philo_routine.c */
void				*philo_routine(void *arg);

/* utils.c */
long long			get_time_ms(void);
void				msleep(long long ms);
void				log_state(t_rules *rules, int id, const char *msg);

#endif


