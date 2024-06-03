#include "philo.h"

int ft_atoi(const char *str)
{
	int i = 0;
	int sign = 1;
	int res = 0;

	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n' || str[i] == '\v' || str[i] == '\f' || str[i] == '\r')
		i++;
	if (str[i] == '-')
	{
		sign = -1;
		i++;
	}
	else if (str[i] == '+')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = res * 10 + str[i] - '0';
		i++;
	}
	return (res * sign);
}

long int get_time_since_start(t_philo *philo)
{
	struct timeval current_time;
	gettimeofday(&current_time, NULL);
	return ((current_time.tv_sec * 1000 + current_time.tv_usec / 1000) - philo->start_time);
}

long int get_time(void)
{
	struct timeval current_time;
	gettimeofday(&current_time, NULL);
	return (current_time.tv_sec * 1000 + current_time.tv_usec / 1000);
}

void *hello_philo(void *p)
{
	t_philo *philo = (t_philo *)p;
			// *philo->death = 1;

	philo->start_time = get_time();
	philo->last_meal = get_time();
	if (philo->id % 2 == 1)
		usleep(1000);
	while (1)
	{
		if (*philo->stop == 1)
			return (NULL);
		pthread_mutex_lock(philo->l_fork);
		if (*philo->stop == 1)
			return (NULL);
		pthread_mutex_lock(philo->r_fork);
		philo->nb_meal++;
		pthread_mutex_lock(philo->print);
		if (philo->last_meal + philo->time_to_die < get_time())
		{
			printf("%ld philo %d is dead\n", get_time_since_start(philo), philo->id);
			*philo->death = 1;
			return (NULL);
		}
		printf("%ld %d is eating\n", get_time_since_start(philo), philo->id);
		pthread_mutex_unlock(philo->print);
		philo->last_meal = get_time();
		usleep(philo->time_to_eat * 1000);
		pthread_mutex_unlock(philo->r_fork);
		pthread_mutex_unlock(philo->l_fork);
		if (*philo->stop == 1)
			return (NULL);
		pthread_mutex_lock(philo->print);
		printf("%ld %d is sleeping\n", get_time_since_start(philo), philo->id);
		pthread_mutex_unlock(philo->print);
		if (*philo->stop == 1)
			return (NULL);
		usleep(philo->time_to_sleep * 1000);
		if (*philo->stop == 1)
			return (NULL);
		pthread_mutex_lock(philo->print);
		printf("%ld %d is thinking\n", get_time_since_start(philo), philo->id);
		pthread_mutex_unlock(philo->print);
		if (*philo->stop == 1)
			return (NULL);
		if (philo->nb_must_eat != -1 && philo->nb_meal >= philo->nb_must_eat)
		{
			*philo->death = 1;
			return (NULL);
		}
	}
}

void *monitoring(void *p)
{
	t_data *data = (t_data *)p;
	int i = 0;
	while (1)
	{
		for (i = 0; i < data->nb_philo; i++)
		{
			if (data->philo[i].last_meal + data->philo[i].time_to_die < get_time())
			{
				pthread_mutex_lock(data->philo[i].print);
				printf("%ld philo %d is dead\n", get_time_since_start(&data->philo[i]), data->philo[i].id);
				pthread_mutex_unlock(data->philo[i].print);
				*data->stop = 1;
				data->death[i] = 1;
				return (NULL);
			}
		}
		for (i = 0; i < data->nb_philo; i++)
		{
		// printf("monitoring death = %d\n", data->death[0]);
			if (data->death[i] == 1)
				return (NULL);
		}
	}
}

void *ft_calloc(size_t count, size_t size)
{
	void *ptr;
	ptr = malloc(count * size);
	if (ptr == NULL)
		return (NULL);
	memset(ptr, 0, count * size);
	return (ptr);
}

int main(int argc, char **argv)
{
	if (argc != 5)
	{
		printf("nope\n");
		return (1);
	}
	long int i = 0;
	int nb_philo = ft_atoi(argv[1]);
	pthread_mutex_t *forks = ft_calloc(nb_philo, sizeof(pthread_mutex_t));
	pthread_t *threads = ft_calloc(nb_philo + 1, sizeof(pthread_t));
	t_data *data = ft_calloc(1, sizeof(t_data));
	pthread_mutex_t *print = ft_calloc(1, sizeof(pthread_mutex_t));
	int *forks_status = ft_calloc(nb_philo, sizeof(int));
	int stop = 0;
	data->philo = ft_calloc(nb_philo, sizeof(t_philo));
	data->death = ft_calloc(nb_philo, sizeof(int));
	data->nb_philo = nb_philo;
	data->stop = &stop;
	for (i = 0; i < nb_philo; i++)
	{
		pthread_mutex_init(&forks[i], NULL);
	}
	pthread_mutex_init(print, NULL);

	for (i = 0; i < nb_philo; i++)
	{
		data->philo[i].id = i;
		data->philo[i].l_fork = &forks[(i - 1 + nb_philo) % nb_philo];
		data->philo[i].r_fork = &forks[i];
		data->philo[i].print = print;
		data->philo[i].forks_status = forks_status;
		data->philo[i].last_meal = 0;
		data->philo[i].time_to_die = ft_atoi(argv[2]);
		data->philo[i].time_to_eat = ft_atoi(argv[3]);
		data->philo[i].time_to_sleep = ft_atoi(argv[4]);
		data->philo[i].nb_meal = 0;
		data->philo[i].start_time = 0;
		data->philo[i].death = data->death;
		data->philo[i].nb_must_eat = -1;
		data->philo[i].stop = &stop;
	}

	for (i = 0; i < nb_philo; i++)
	{
		pthread_create(&threads[i], NULL, hello_philo, &data->philo[i]);
	}
	usleep(1000);
	pthread_create(&threads[nb_philo], NULL, monitoring, data);

	for (i = 0; i < nb_philo; i++)
	{
		pthread_join(threads[i], NULL);
	}

	pthread_join(threads[nb_philo], NULL);

	for (i = 0; i < nb_philo; i++)
	{
		pthread_mutex_destroy(&forks[i]);
	}
	pthread_mutex_destroy(print);

	free(forks);
	free(threads);
	free(print);
	free(forks_status);
	free(data->philo);
	free(data->death);
	free(data);

	return (0);
}
