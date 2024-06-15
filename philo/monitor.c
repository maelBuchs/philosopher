/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbuchs <mbuchs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 22:09:20 by mbuchs            #+#    #+#             */
/*   Updated: 2024/06/15 22:32:35 by mbuchs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	create_and_join_threads(t_data *data, pthread_t *threads)
{
	int	i;
	int	j;

	j = 0;
	i = 0;
	pthread_mutex_lock(&data->start_mutex);
	while (i < data->nb_philo)
	{
		if (pthread_create(&threads[i], NULL, hello_philo, &data->philo[i]))
			free_all_thread(threads, i, data);
		i++;
	}
	pthread_mutex_unlock(&data->start_mutex);
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

int	monitoring_must_eat(t_data *data)
{
	int	i;

	i = 0;
	pthread_mutex_lock(&data->start_mutex);
	while (i < data->nb_philo && data->nb_must_eat != -1
		&& data->philo[i].nb_meal >= data->nb_must_eat)
	{
		i++;
	}
	pthread_mutex_unlock(&data->start_mutex);
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
	int		i;
	long	current_time;

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
	t_data	*data;

	data = (t_data *)p;
	while (1)
	{
		if (monitoring_last_eat(data))
			return (NULL);
		if (monitoring_must_eat(data))
			return (NULL);
		usleep(1000);
	}
}
