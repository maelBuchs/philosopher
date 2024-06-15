/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbuchs <mbuchs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 22:12:06 by mbuchs            #+#    #+#             */
/*   Updated: 2024/06/15 22:47:31 by mbuchs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

void	mutex_get_time(t_philo *philo)
{
	pthread_mutex_lock(&philo->time_mutex);
	philo->start_time = get_time();
	philo->last_meal = philo->start_time;
	pthread_mutex_unlock(&philo->time_mutex);
}
