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
void	debug_print_rules(t_rules rules);

int	main(int argc, char **argv)
{
	t_rules	rules;
	t_philo	*philos;
	int		status;

	if ((argc != 5) && (argc != 6))
		return (1);
	if (!parse_args(argc, argv, &rules))
		return (1);
	if (pthread_mutex_init(&rules.death_mutex, NULL) != 0)
		return (1);
	philos = prep_table(rules.number_of_philosophers, &rules);
	if (!philos)
		return (1);
	debug_print_rules(rules);
	debug_print_rules(*philos->rules);
	status = 0;
	status = start_simulation(philos);
	terminate_simulation(philos);
	return (status);
}

void	debug_print_rules(t_rules rules)
{
	printf("number of philos: %i\n", rules.number_of_philosophers);
	printf("number philos must eat: %i\n", rules.number_of_times_each_philosopher_must_eat);
	printf("someone died: %i\n", rules.someone_died);
	printf("start time: %lld\n", rules.start_time);
	printf("time to die: %i\n", rules.time_to_die);
	printf("time to eat: %i\n", rules.time_to_eat);
	printf("time to sleep: %i\n\n\n", rules.time_to_sleep);
}

bool	parse_args(int argc, char **argv, t_rules *rules)
{
	memset((void *)rules, 0, sizeof(t_rules));
	rules->number_of_philosophers = atoi_positive(argv[1]);
	rules->time_to_die = atoi_positive(argv[2]);
	rules->time_to_eat = atoi_positive(argv[3]);
	rules->time_to_sleep = atoi_positive(argv[4]);
	if (argc == 6)
		rules->number_of_times_each_philosopher_must_eat = atoi_positive(argv[5]);
	else
		rules->number_of_times_each_philosopher_must_eat = NO_ARG;
	if (rules->number_of_philosophers == -1
		|| rules->time_to_die == -1
		|| rules->time_to_eat == -1
		|| rules->time_to_sleep == -1
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
	while(*str)
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
	while (++id <= amount)
	{
		new_philo = new_head_and_fork(id, rules);
		if (!new_philo)
		{
			terminate_simulation(philos);
			return (NULL);
		}
		cdll_add(&philos, new_philo);
	}
	return (philos);
}

void	cdll_add(t_philo **ptr, t_philo *new)
{
	t_philo	*philos;

	philos = *ptr;
	if (new->id == 1)
	{
		philos = new;
		philos->left_fork = philos->right_fork;
		philos->left_fork->right_philo = philos;
		return ;
	}
	new->right_fork->right_philo = philos;
	new->left_fork = philos->left_fork;
	philos->left_fork->right_philo = new;
	philos->left_fork = new->right_fork;
}

t_philo	*new_head_and_fork(int id, t_rules *rules)
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
	if (pthread_mutex_init(&philosopher->right_fork->mutex, NULL) != 0)
	{
		free(philosopher);
		return (NULL);
	}
	philosopher->id = id;
	philosopher->right_fork->id = id;
	philosopher->right_fork->left_philo = philosopher;
	return (philosopher);
}


int	start_simulation(t_philo *philos)
{
	t_philo	*head;
	int		n_philos;

	n_philos = philos->rules->number_of_philosophers;
	head = philos;
	philos->rules->start_time = time_now_ms();
	while (n_philos--)
	{
		if (pthread_create(&head->thread, NULL, routine, (void *)head) != 0)
			return (ft_putstr_error("pthread create error\n"));
		head = head->right_fork->right_philo;
	}
	head = philos;
	n_philos = philos->rules->number_of_philosophers;
	while (n_philos--)
	{
		if (pthread_join(head->thread, NULL) != 0)
			return (ft_putstr_error("join error\n"));
		head = head->right_fork->right_philo;
	}
	if (pthread_create(&philos->rules->monitor_thread, NULL, watchdog,
			(void *)philos) != 0)
		return (ft_putstr_error("pthread create error\n"));
	return (pthread_detach(philos->rules->monitor_thread));
}

void	*watchdog(void *philos)
{
	t_philo	*philosopher;

	if (!philos)
		return (NULL);
	philosopher = (t_philo *)philos;
	while (!is_dead(philosopher) && !ate_enough(philosopher))
	{
		philosopher = philosopher->right_fork->right_philo;
		if (!philosopher)
			break ;
	}
	return (NULL);
}

int	ft_putstr_error(char *s)
{
	size_t	buffer;
	char	*ptr;

	if (!s)
		return (1);
	ptr = s;
	buffer = 0;
	while (*s)
	{
		buffer++;
		s++;
	}
	write(2, ptr, buffer);
	return (1);
}

void	*routine(void *arg)
{
	t_philo	*philosopher;

	philosopher = (t_philo *)arg;
	if (!philosopher)
		return (NULL);
	philosopher->last_meal = time_now_ms();
	if (philosopher->rules->number_of_philosophers == 1)
	{
		one_philo(philosopher);
		return (NULL);
	}
	while (1)
	{
		if (should_stop(philosopher))
			return (NULL);
		think(philosopher);
		take_forks(philosopher);
		eat(philosopher);
		place_forks(philosopher);
		if (should_stop(philosopher))
			return (NULL);
		philo_sleep(philosopher);
	}
	return (NULL);
}

void	philo_sleep(t_philo *philosopher)
{
	long long	now;
	long long	start;

	if (!philosopher)
		return ;
	start = philosopher->rules->start_time;
	now = time_now_ms();
	printf("%lld %i is sleeping\n", now - start, philosopher->id);
	usleep(1000 * philosopher->rules->time_to_sleep);
}

void	think(t_philo *philosopher)
{
	long long	now;
	long long	start;

	if (!philosopher)
		return ;
	start = philosopher->rules->start_time;
	now = time_now_ms();
	printf("%lld %i is thinking\n", now - start, philosopher->id);
}

void	one_philo(t_philo *philosopher)
{
	long long	now;
	long long	start;

	start = philosopher->rules->start_time;
	now = time_now_ms();
	pthread_mutex_lock(&philosopher->right_fork->mutex);
	printf("%lld %i has taken a fork\n", now - start, philosopher->id);
	usleep(philosopher->rules->time_to_die * 1000);
	is_dead(philosopher);
	pthread_mutex_unlock(&philosopher->right_fork->mutex);
	return ;
}

bool	ate_enough(t_philo *philosopher)
{
	int	meals_eaten;
	int	must_eat;

	must_eat = philosopher->rules->number_of_times_each_philosopher_must_eat;
	if (must_eat == NO_ARG)
		return (false);
	pthread_mutex_lock(&philosopher->eat_mutex);
	meals_eaten = philosopher->meals_eaten;
	pthread_mutex_unlock(&philosopher->eat_mutex);
	if (meals_eaten >= must_eat)
	{
		return (true);
	}
	return (false);
}

bool	should_stop(t_philo *philosopher)
{
	bool	enough_eat;
	bool	someone_died;

	pthread_mutex_lock(&philosopher->rules->death_mutex);
	someone_died = philosopher->rules->someone_died;
	pthread_mutex_unlock(&philosopher->rules->death_mutex);
	enough_eat = ate_enough(philosopher);
	return (someone_died || enough_eat);
}

void	eat(t_philo *philosopher)
{
	long long	now;
	long long	start;

	start = philosopher->rules->start_time;
	if ((int)(now - philosopher->last_meal) <= philosopher->rules->time_to_die)
	{
		pthread_mutex_lock(&philosopher->eat_mutex);
		now = time_now_ms();
		philosopher->last_meal = now;
		printf("%lld %i is eating\n", now - start, philosopher->id);
		usleep(philosopher->rules->time_to_eat * 1000);
		philosopher->meals_eaten++;
		pthread_mutex_unlock(&philosopher->eat_mutex);
	}
}

long long	time_now_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000LL + tv.tv_usec / 1000LL);
}

void	place_forks(t_philo *philosopher)
{
	int	philosopher_id;

	philosopher_id = philosopher->id;
	if (philosopher_id % 2 == 0)
	{
		pthread_mutex_unlock(&philosopher->left_fork->mutex);
		pthread_mutex_unlock(&philosopher->right_fork->mutex);
	}
	else
	{
		pthread_mutex_unlock(&philosopher->right_fork->mutex);
		pthread_mutex_unlock(&philosopher->left_fork->mutex);
	}
}

void	take_forks(t_philo *philosopher)
{
	int			philosopher_id;
	long long	now;
	long long	start;

	start = philosopher->rules->start_time;
	philosopher_id = philosopher->id;
	if (philosopher_id % 2 == 0)
	{
		pthread_mutex_lock(&philosopher->right_fork->mutex);
		now = time_now_ms();
		printf("%lld %i has taken a fork\n", now - start, philosopher_id);
		pthread_mutex_lock(&philosopher->left_fork->mutex);
		now = time_now_ms();
		printf("%lld %i has taken a fork\n", now - start, philosopher_id);
	}
	else
	{
		pthread_mutex_lock(&philosopher->left_fork->mutex);
		now = time_now_ms();
		printf("%lld %i has taken a fork\n", now - start, philosopher_id);
		pthread_mutex_lock(&philosopher->right_fork->mutex);
		now = time_now_ms();
		printf("%lld %i has taken a fork\n", now - start, philosopher_id);
	}
}

bool	is_dead(t_philo *philosopher)
{
	long long	now;
	long long	start;
	long long	last_meal;

	start = philosopher->rules->start_time;
	now = time_now_ms();
	pthread_mutex_lock(&philosopher->eat_mutex);
	last_meal = philosopher->last_meal;
	pthread_mutex_unlock(&philosopher->eat_mutex);
	if ((int)(now - last_meal) > philosopher->rules->time_to_die)
	{
		pthread_mutex_lock(&philosopher->rules->death_mutex);
		if (philosopher->rules->someone_died == false)
		{
			philosopher->rules->someone_died = true;
			printf("%lld %i died\n", now - start, philosopher->id);
		}
		pthread_mutex_unlock(&philosopher->rules->death_mutex);
		return (true);
	}
	return (false);
}

void	terminate_simulation(t_philo *philos)
{
	t_philo	*philo_ptr;
	t_fork	*fork_ptr;
	int		n_philos;
	int		i;

	if (!philos)
		return ;
	n_philos = philos->left_fork->left_philo->id;
	i = 0;
	while (i < n_philos)
	{
		philo_ptr = philos;
		fork_ptr = philos->right_fork;
		philos = philos->right_fork->right_philo;
		free(philo_ptr);
		free(fork_ptr);
		i++;
	}
}
