/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbuchs <mbuchs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 22:05:49 by mbuchs            #+#    #+#             */
/*   Updated: 2024/06/15 22:09:09 by mbuchs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	philosophers(t_data *data, pthread_mutex_t *forks)
{
	pthread_t	*threads;

	threads = ft_calloc(data->nb_philo + 1, sizeof(pthread_t));
	create_and_join_threads(data, threads);
	destroy_mutexes(data, forks);
	free_all(forks, threads, data);
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
	if (philo->id % 2)
	{
		usleep(1000);
		pthread_mutex_lock(philo->l_fork);
		pthread_mutex_lock(philo->r_fork);
	}
	else
	{
		pthread_mutex_lock(philo->r_fork);
		pthread_mutex_lock(philo->l_fork);
	}
	mutexed_print("has taken a fork\n", philo);
	mutexed_print("has taken a fork\n", philo);
	mutexed_print("is eating\n", philo);
	pthread_mutex_lock(philo->start_mutex);
	philo->nb_meal++;
	pthread_mutex_unlock(philo->start_mutex);
	pthread_mutex_lock(&philo->time_mutex);
	philo->last_meal = get_time();
	pthread_mutex_unlock(&philo->time_mutex);
	usleep(philo->time_to_eat * 1000);
	pthread_mutex_unlock(philo->l_fork);
	pthread_mutex_unlock(philo->r_fork);
}

void	*hello_philo(void *p)
{
	t_philo	*philo;

	philo = (t_philo *)p;
	pthread_mutex_lock(philo->start_mutex);
	pthread_mutex_unlock(philo->start_mutex);
	mutex_get_time(philo);
	if (philo->id % 2)
		usleep(1000);
	while (1)
	{
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
