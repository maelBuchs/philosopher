/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+
	+:+     */
/*   By: mbuchs <mbuchs@student.42.fr>              +#+  +:+
	+#+        */
/*                                                +#+#+#+#+#+
	+#+           */
/*   Created: 2024/06/08 09:40:02 by mbuchs            #+#    #+#             */
/*   Updated: 2024/06/08 09:40:02 by mbuchs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"


void	ft_exit(char *str)
{
	printf("%s\n", str);
	exit(1);
}

int	ft_atoi(const char *str)
{
	int		i;
	int		neg;
	long	nb;

	i = 0;
	neg = 0;
	nb = 0;
	while (str[i] == ' ' || str[i] == '\t'
		|| str[i] == '\n' || str[i] == '\v'
		|| str[i] == '\f' || str[i] == '\r')
		i++;
	if (str[i] == '-')
		neg = -1;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		nb = nb * 10 + (str[i] - '0');
		if (nb > 2147483647 && neg == 1)
			return (-1);
		if (nb > 2147483648 && neg == -1)
			return (0);
		i++;
	}
	return (nb * neg);
}

long int	get_time_since_start(t_philo *philo)
{
	struct timeval	current_time;
	long int		start_time_copy;

	gettimeofday(&current_time, NULL);
	pthread_mutex_lock(&philo->time_mutex);
	start_time_copy = philo->start_time;
	pthread_mutex_unlock(&philo->time_mutex);

	return ((current_time.tv_sec * 1000 + current_time.tv_usec / 1000)
		- start_time_copy);
}

long int	get_time(void)
{
	struct timeval	current_time;

	gettimeofday(&current_time, NULL);
	return (current_time.tv_sec * 1000 + current_time.tv_usec / 1000);
}

void	mutexed_print(char *str, t_philo *philo)
{
	pthread_mutex_lock(philo->print);
	printf("%ld %d %s", get_time_since_start(philo), philo->id, str);
	pthread_mutex_unlock(philo->print);
}

int	philo_sleep_and_think(t_philo *philo)
{
	pthread_mutex_lock(philo->stop_mutex);
	if (*philo->stop == 1)
	{
		pthread_mutex_unlock(philo->stop_mutex);
		return (1);
	}
	pthread_mutex_unlock(philo->stop_mutex);

	mutexed_print("is sleeping\n", philo);
	usleep(philo->time_to_sleep * 1000);

	pthread_mutex_lock(philo->stop_mutex);
	if (*philo->stop == 1)
	{
		pthread_mutex_unlock(philo->stop_mutex);
		return (1);
	}
	pthread_mutex_unlock(philo->stop_mutex);

	mutexed_print("is thinking\n", philo);
	return (0);
}

void	philo_l_first(t_philo *philo)
{
	pthread_mutex_lock(philo->l_fork);
	pthread_mutex_lock(philo->stop_mutex);
	if (*philo->stop == 1)
	{
		pthread_mutex_unlock(philo->stop_mutex);
		pthread_mutex_unlock(philo->l_fork);
		return ;
	}
	pthread_mutex_unlock(philo->stop_mutex);

	mutexed_print("has taken a fork\n", philo);
	pthread_mutex_lock(philo->r_fork);
	pthread_mutex_lock(philo->stop_mutex);
	if (*philo->stop == 1)
	{
		pthread_mutex_unlock(philo->stop_mutex);
		pthread_mutex_unlock(philo->r_fork);
		pthread_mutex_unlock(philo->l_fork);
		return ;
	}
	pthread_mutex_unlock(philo->stop_mutex);

	mutexed_print("has taken a fork\n", philo);
}

void	philo_r_first(t_philo *philo)
{
	pthread_mutex_lock(philo->r_fork);
	pthread_mutex_lock(philo->stop_mutex);
	if (*philo->stop == 1)
	{
		pthread_mutex_unlock(philo->stop_mutex);
		pthread_mutex_unlock(philo->r_fork);
		return ;
	}
	pthread_mutex_unlock(philo->stop_mutex);

	mutexed_print("has taken a fork\n", philo);
	pthread_mutex_lock(philo->l_fork);
	pthread_mutex_lock(philo->stop_mutex);
	if (*philo->stop == 1)
	{
		pthread_mutex_unlock(philo->stop_mutex);
		pthread_mutex_unlock(philo->l_fork);
		pthread_mutex_unlock(philo->r_fork);
		return ;
	}
	pthread_mutex_unlock(philo->stop_mutex);

	mutexed_print("has taken a fork\n", philo);
}

void	philo_eat(t_philo *philo)
{
	mutexed_print("is eating\n", philo);
	pthread_mutex_lock(&philo->time_mutex);
	philo->nb_meal++;
	philo->last_meal = get_time();
	pthread_mutex_unlock(&philo->time_mutex);

	usleep(philo->time_to_eat * 1000);
	pthread_mutex_unlock(philo->l_fork);
	pthread_mutex_unlock(philo->r_fork);
}

void	*hello_philo(void *p)
{
	t_philo *philo;

	philo = (t_philo *)p;
	pthread_mutex_lock(&philo->time_mutex);
	philo->start_time = get_time();
	philo->last_meal = philo->start_time;
	pthread_mutex_unlock(&philo->time_mutex);

	if (philo->id % 2 == 1)
		usleep(1000);
	while (1)
	{
		if (philo->l_fork < philo->r_fork)
			philo_l_first(philo);
		else
			philo_r_first(philo);
		pthread_mutex_lock(philo->stop_mutex);
		if (*philo->stop == 1)
		{
			pthread_mutex_unlock(philo->stop_mutex);
			return (NULL);
		}
		pthread_mutex_unlock(philo->stop_mutex);

		philo_eat(philo);
		if (philo_sleep_and_think(philo) == 1)
			return (NULL);
	}
}

int	monitoring_must_eat(t_data *data)
{
	int i;

	i = 0;
	while (i < data->nb_philo && data->nb_must_eat != -1
		&& data->philo[i].nb_meal >= data->nb_must_eat)
	{
		i++;
	}
	if (i == data->nb_philo)
	{
		pthread_mutex_lock(&data->stop_mutex);
		*data->stop = 1;
		pthread_mutex_unlock(&data->stop_mutex);
		return (1);
	}
	return (0);
}

int	monitoring_last_eat(t_data *data)
{
	int i;
	long current_time;

	current_time = get_time();
	i = 0;
	while (i < data->nb_philo)
	{
		pthread_mutex_lock(&data->philo[i].time_mutex);
		if (data->philo[i].last_meal
			+ data->philo[i].time_to_die < current_time)
		{
			pthread_mutex_unlock(&data->philo[i].time_mutex);
			mutexed_print("is dead\n", &data->philo[i]);
			pthread_mutex_lock(&data->stop_mutex);
			*data->stop = 1;
			pthread_mutex_unlock(&data->stop_mutex);
			return (1);
		}
		pthread_mutex_unlock(&data->philo[i].time_mutex);
		i++;
	}
	return (0);
}

void	*monitoring(void *p)
{
	t_data *data;

	data = (t_data *)p;
	while (1)
	{
		if (monitoring_last_eat(data))
			return (NULL);
		if (monitoring_must_eat(data))
			return (NULL);
	}
}

void	*ft_calloc(size_t count, size_t size)
{
	void *ptr;

	ptr = malloc(count * size);
	if (ptr == NULL)
		return (NULL);
	memset(ptr, 0, count * size);
	return (ptr);
}

int	parsing(int ac, char **av, t_data *data)
{
	if (ac != 5 && ac != 6)
		ft_exit("invalid number of arguments\n");

	data->nb_philo = ft_atoi(av[1]);
	if (data->nb_philo < 1 || data->nb_philo > 200)
		ft_exit("invalid number of philosophers\n");

	data->stop = ft_calloc(1, sizeof(int));
	pthread_mutex_init(&data->stop_mutex, NULL);
	data->nb_must_eat = -1;
	if (ac == 6)
		data->nb_must_eat = ft_atoi(av[5]);

	data->philo = ft_calloc(data->nb_philo, sizeof(t_philo));
	data->death = ft_calloc(data->nb_philo, sizeof(int));
	pthread_mutex_init(&data->print, NULL);

	return (0);
}

void	free_all(pthread_mutex_t *forks, pthread_t *threads, t_data *data)
{
	free(forks);
	free(threads);
	pthread_mutex_destroy(&data->print);
	free(data->philo);
	free(data->death);
	pthread_mutex_destroy(&data->stop_mutex);
	free(data->stop);
	free(data);
}

int	create_and_join_threads(t_data *data, pthread_t *threads)
{
	int i;

	i = 0;
	while (i < data->nb_philo)
	{
		pthread_create(&threads[i], NULL, hello_philo, &data->philo[i]);
		i++;
	}
	usleep(1000);
	pthread_create(&threads[data->nb_philo], NULL, monitoring, data);
	i = 0;
	while (i < data->nb_philo)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
	pthread_join(threads[data->nb_philo], NULL);
	return (0);
}

int	init_mutexes(t_data *data, pthread_mutex_t *forks)
{
	int i;

	i = 0;
	while (i < data->nb_philo)
	{
		pthread_mutex_init(&forks[i], NULL);
		pthread_mutex_init(&data->philo[i].time_mutex, NULL);
		i++;
	}
	return (0);
}

void	destroy_mutexes(t_data *data, pthread_mutex_t *forks)
{
	int i;

	i = 0;
	while (i < data->nb_philo)
	{
		pthread_mutex_destroy(&forks[i]);
		pthread_mutex_destroy(&data->philo[i].time_mutex);
		i++;
	}
}

int	philosophers(t_data *data, pthread_mutex_t *forks)
{
	pthread_t *threads;

	threads = ft_calloc(data->nb_philo + 1, sizeof(pthread_t));
	create_and_join_threads(data, threads);
	destroy_mutexes(data, forks);
	free_all(forks, threads, data);
	return (0);
}

int	init_all(t_data **data, pthread_mutex_t **forks)
{
	*forks = ft_calloc((*data)->nb_philo, sizeof(pthread_mutex_t));
	init_mutexes((*data), (*forks));
	return (0);
}

void	philo_init(t_philo *philo, int i)
{
	philo->id = i;
	pthread_mutex_lock(&philo->time_mutex);
	philo->last_meal = 0;
	philo->nb_meal = 0;
	philo->start_time = 0;
	pthread_mutex_unlock(&philo->time_mutex);
	philo->nb_must_eat = -1;
	pthread_mutex_init(&philo->time_mutex, NULL);
}

int	main(int argc, char **argv)
{
	int i;
	t_data *data;
	pthread_mutex_t *forks;

	i = 0;
	data = ft_calloc(1, sizeof(t_data));
	parsing(argc, argv, data);
	init_all(&data, &forks);
	while (i < data->nb_philo)
	{
		philo_init(&data->philo[i], i);
		data->philo[i].l_fork = &forks[(i - 1 + data->nb_philo)
			% data->nb_philo];
		data->philo[i].r_fork = &forks[i];
		data->philo[i].print = &data->print;
		data->philo[i].time_to_die = ft_atoi(argv[2]);
		data->philo[i].time_to_eat = ft_atoi(argv[3]);
		data->philo[i].time_to_sleep = ft_atoi(argv[4]);
		data->philo[i].stop = data->stop;
		data->philo[i].stop_mutex = &data->stop_mutex;
		i++;
	}
	philosophers(data, forks);
	return (0);
}
