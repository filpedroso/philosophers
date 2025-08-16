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

void		debug_print_rules(t_rules rules);
void	debug_print_philos_list(t_philo *philos);

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
	// debug_print_rules(rules);
	philos = prep_table(rules.number_of_philosophers, &rules);
	if (!philos)
		return (1);
	// debug_print_rules(*philos->rules);
	status = 0;
	// debug_print_philos_list(philos);
	status = start_simulation(philos);
	terminate_simulation(philos);
	return (status);
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
		new_philo = new_philo_fork_pair(id);
		if (!new_philo)
		{
			terminate_simulation(philos);
			return (NULL);
		}
		new_philo->rules = rules;
		cdll_add(&philos, new_philo);
	}
	// printf("debug: new_philo working\n");
	return (philos);
}

void	cdll_add(t_philo **ptr, t_philo *new)
{
	if (!*ptr)
	{
		*ptr = new;
		new->left_fork = new->right_fork;
		new->right_fork->right_philo = new;
		return ;
	}
	new->right_fork->right_philo = (*ptr);
	new->left_fork = (*ptr)->left_fork;
	(*ptr)->left_fork->right_philo = new;
	(*ptr)->left_fork = new->right_fork;
}

t_philo	*new_philo_fork_pair(int id)
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
	if (pthread_mutex_init(&philosopher->right_fork->mutex, NULL) != 0
		|| pthread_mutex_init(&philosopher->eat_mutex, NULL) != 0)
	{
		free(philosopher);
		return (NULL);
	}
	philosopher->id = id;
	philosopher->right_fork->id = id;
	philosopher->right_fork->left_philo = philosopher;
	philosopher->right_fork->being_used = false;
	return (philosopher);
}

int	start_simulation(t_philo *philos)
{
	int	n_philos;

	n_philos = philos->rules->number_of_philosophers;
	philos->rules->start_time = time_now_ms();
	if (!create_threads(philos, n_philos))
		return (1);
	if (!create_monitor_detached(philos))
		return (1);
	if (!join_philos(philos, n_philos))
		return (1);
	return (0);
}

bool	join_philos(t_philo *philos, int n_philos)
{
	t_philo	*head;

	head = philos;
	while (n_philos--)
	{
		if (pthread_join(head->thread, NULL) != 0)
			return (ft_putstr_error("join error\n"));
		head = head->right_fork->right_philo;
	}
	return (true);
}

bool	create_monitor_detached(t_philo *philos)
{
	if (pthread_create(&philos->rules->monitor_thread, NULL, watchdog,
			(void *)philos) != 0)
	{
		ft_putstr_error("pthread monitor create error\n");
		return (false);
	}
	if (pthread_detach(philos->rules->monitor_thread) != 0)
	{
		ft_putstr_error("pthread monitor detach error\n");
		return (false);
	}
	return (true);
}

bool	create_threads(t_philo *philos, int n_philos)
{
	t_philo	*head;

	head = philos;
	while (n_philos--)
	{
		if (pthread_create(&head->thread, NULL, routine, (void *)head) != 0)
		{
			ft_putstr_error("pthread philo create error\n");
			return (false);
		}
		head = head->right_fork->right_philo;
	}
	pthread_mutex_init(&philos->rules->simul_mutex, NULL);
	pthread_mutex_lock(&philos->rules->simul_mutex);
	philos->rules->simulation_started = true;
	pthread_mutex_unlock(&philos->rules->simul_mutex);
	return (true);
}

void	*watchdog(void *philos)
{
	t_philo	*philosopher;
	int		i;

	if (!philos)
		return (NULL);
	philosopher = (t_philo *)philos;
	while(1)
	{
		i = -1;
		while (++i < philosopher->rules->number_of_philosophers)
		{
			if (is_dead(philosopher))
			{
				printf("%lld %i died\n", time_now_ms()
					- philosopher->rules->start_time, philosopher->id);
				return (NULL);
			}
			philosopher = philosopher->right_fork->right_philo;
		}
		sleep_millisecs(5);
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
	while (simulation_has_started(philosopher->rules) == false)
		usleep(500);
	philosopher->last_meal = time_now_ms();
	if (philosopher->rules->number_of_philosophers == 1)
		return (one_philo(philosopher), NULL);
	while (1)
	{
		if (!eat(philosopher))
			return (NULL);
		if (!philo_sleep(philosopher))
			return (NULL);
		if (!think(philosopher))
			return (NULL);
	}
	return (NULL);
}

bool	simulation_has_started(t_rules *rules)
{
	bool	has_started;

	pthread_mutex_lock(&rules->simul_mutex);
	has_started = rules->simulation_started;
	pthread_mutex_unlock(&rules->simul_mutex);
	return (has_started);
}

bool	is_starving(t_philo *philosopher)
{
	bool	is_starving;
	int		base_time_left;
	int		base_time_spent;

	base_time_spent = philosopher->rules->time_to_eat
		+ philosopher->rules->time_to_sleep;
	base_time_left = philosopher->rules->time_to_die - base_time_spent;
	pthread_mutex_lock(&philosopher->eat_mutex);
	is_starving = (time_now_ms() - philosopher->last_meal)
		> (base_time_spent + base_time_left / 3);
	pthread_mutex_unlock(&philosopher->eat_mutex);
	return (is_starving);
}

bool	philo_sleep(t_philo *philosopher)
{
	long long	start;

	if (!philosopher)
		return (false);
	if (should_stop(philosopher))
		return (false);
	start = philosopher->rules->start_time;
	printf("%lld %i is sleeping\n", time_now_ms() - start, philosopher->id);
	sleep_millisecs(philosopher->rules->time_to_sleep);
	return (true);
}

bool	think(t_philo *philosopher)
{
	long long	now;
	long long	start;
	int			id;

	if (!philosopher)
		return (false);
	if (should_stop(philosopher))
		return (false);
	start = philosopher->rules->start_time;
	now = time_now_ms();
	id = philosopher->id;
	printf("%lld %i is thinking\n", now - start, id);
	while (!is_starving(philosopher))
		sleep_millisecs(30);
	return (true);
}

void	one_philo(t_philo *philosopher)
{
	long long	now;
	long long	start;

	start = philosopher->rules->start_time;
	now = time_now_ms();
	pthread_mutex_lock(&philosopher->right_fork->mutex);
	printf("%lld %i has taken a fork\n", now - start, philosopher->id);
	sleep_millisecs(philosopher->rules->time_to_die);
	is_dead(philosopher);
	printf("%lld %i died\n", now - start, philosopher->id);
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
	bool	someone_died;
	
	someone_died = false;
	pthread_mutex_lock(&philosopher->rules->death_mutex);
	someone_died = philosopher->rules->someone_died;
	pthread_mutex_unlock(&philosopher->rules->death_mutex);
	if (someone_died)
		return (true);
	if (philosopher->rules->number_of_times_each_philosopher_must_eat != NO_ARG)
		return (ate_enough(philosopher));
	return (false);
}

bool	eat(t_philo *philosopher)
{
	long long	start;

	start = philosopher->rules->start_time;
	if ((time_now_ms() - philosopher->last_meal)
		<= (long long)philosopher->rules->time_to_die
		&& (should_stop(philosopher) == false))
	{

		take_forks(philosopher);
		pthread_mutex_lock(&philosopher->eat_mutex);
		philosopher->last_meal = time_now_ms();
		pthread_mutex_unlock(&philosopher->eat_mutex);
		if (should_stop(philosopher))
			return (false);
		printf("%lld %i is eating\n", philosopher->last_meal - start, philosopher->id);
		sleep_millisecs(philosopher->rules->time_to_eat);
		place_forks(philosopher);
		pthread_mutex_lock(&philosopher->eat_mutex);
		philosopher->meals_eaten++;
		pthread_mutex_unlock(&philosopher->eat_mutex);
	}
	else
		return (false);
	return (true);
}

void	sleep_millisecs(long long milliseconds)
{
    long	start;

	start = time_now_ms();
    while ((time_now_ms() - start) < milliseconds)
        usleep(500);
}


long long	time_now_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000LL + tv.tv_usec / 1000LL);
}

void	place_forks(t_philo *philosopher)
{
	pthread_mutex_lock(&philosopher->left_fork->mutex);
	philosopher->left_fork->being_used = false;
	pthread_mutex_unlock(&philosopher->left_fork->mutex);
	pthread_mutex_lock(&philosopher->right_fork->mutex);
	philosopher->right_fork->being_used = false;
	pthread_mutex_unlock(&philosopher->right_fork->mutex);
}

void take_forks(t_philo *philosopher)
{
    long long start = philosopher->rules->start_time;
    pthread_mutex_t *first;
    pthread_mutex_t *second;

    if (philosopher->left_fork->id < philosopher->right_fork->id)
    {
        first = &philosopher->left_fork->mutex;
        second = &philosopher->right_fork->mutex;
    }
    else
    {
        first = &philosopher->right_fork->mutex;
        second = &philosopher->left_fork->mutex;
    }

    while (1)
    {
        pthread_mutex_lock(first);
        pthread_mutex_lock(second);
        if (!philosopher->left_fork->being_used && !philosopher->right_fork->being_used)
        {
            if (should_stop(philosopher))
            {
                pthread_mutex_unlock(first);
                pthread_mutex_unlock(second);
                return;
            }
            philosopher->left_fork->being_used = true;
            philosopher->right_fork->being_used = true;
            printf("%lld %i has taken a fork\n", time_now_ms() - start, philosopher->id);
            printf("%lld %i has taken a fork\n", time_now_ms() - start, philosopher->id);
            pthread_mutex_unlock(first);
            pthread_mutex_unlock(second);
            break;
        }
        pthread_mutex_unlock(first);
        pthread_mutex_unlock(second);
        usleep(100);
    }
}


bool	is_dead(t_philo *philosopher)
{
	long long	last_meal;

	pthread_mutex_lock(&philosopher->eat_mutex);
	last_meal = philosopher->last_meal;
	pthread_mutex_unlock(&philosopher->eat_mutex);
	if ((time_now_ms() - last_meal)
		> (long long)philosopher->rules->time_to_die)
	{
		if (philosopher->rules->someone_died == false)
		{
			pthread_mutex_lock(&philosopher->rules->death_mutex);
			philosopher->rules->someone_died = true;
			pthread_mutex_unlock(&philosopher->rules->death_mutex);
		}
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
	pthread_mutex_destroy(&philos->rules->death_mutex);
	while (i < n_philos)
	{
		philo_ptr = philos;
		fork_ptr = philos->right_fork;
		pthread_mutex_destroy(&fork_ptr->mutex);
		pthread_mutex_destroy(&philo_ptr->eat_mutex);
		philos = philos->right_fork->right_philo;
		free(philo_ptr);
		free(fork_ptr);
		i++;
	}
}
