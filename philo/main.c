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

int	init_mutexes(t_data *data, pthread_mutex_t *forks)
{
	int	i;

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
	int	i;

	i = 0;
	while (i < data->nb_philo)
	{
		pthread_mutex_destroy(&forks[i]);
		pthread_mutex_destroy(&data->philo[i].time_mutex);
		i++;
	}
}

int	init_all(t_data **data, pthread_mutex_t **forks)
{
	*forks = ft_calloc((*data)->nb_philo, sizeof(pthread_mutex_t));
	init_mutexes((*data), (*forks));
	pthread_mutex_init(&(*data)->start_mutex, NULL);
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
	int				i;
	t_data			*d;
	pthread_mutex_t	*f;

	i = 0;
	d = ft_calloc(1, sizeof(t_data));
	parsing(argc, argv, &d, &f);
	while (i < d->nb_philo)
	{
		philo_init(&d->philo[i], i);
		d->philo[i].l_fork = &f[(i - 1 + d->nb_philo) % d->nb_philo];
		d->philo[i].r_fork = &f[i];
		d->philo[i].print = &d->print;
		d->philo[i].time_to_die = ft_atoi(argv[2]);
		d->philo[i].time_to_eat = ft_atoi(argv[3]);
		d->philo[i].time_to_sleep = ft_atoi(argv[4]);
		d->philo[i].start_time = get_time();
		d->philo[i].last_meal = get_time();
		d->philo[i].stop = d->stop;
		d->philo[i].stop_mutex = &d->stop_mutex;
		d->philo[i].start_mutex = &d->start_mutex;
		i++;
	}
	philosophers(d, f);
	return (0);
}
