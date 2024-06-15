/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbuchs <mbuchs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 21:29:24 by mbuchs            #+#    #+#             */
/*   Updated: 2024/06/15 22:47:15 by mbuchs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_exit(char *str, pthread_mutex_t *forks,
				pthread_t *threads, t_data *data)
{
	free_all(forks, threads, data);
	if (str)
		printf("%s\n", str);
	exit(1);
}

int	ft_atoi(const char *str)
{
	int	i;
	int	neg;
	int	nb;

	i = 0;
	neg = 1;
	nb = 0;
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n'
		|| str[i] == '\v' || str[i] == '\f' || str[i] == '\r')
		i++;
	if (str[i] == '-')
	{
		neg = -1;
		i++;
	}
	else if (str[i] == '+')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		nb = nb * 10 + str[i] - '0';
		i++;
	}
	return (nb * neg);
}

void	free_all(pthread_mutex_t *forks, pthread_t *threads, t_data *data)
{
	if (forks)
		free(forks);
	if (threads)
		free(threads);
	if (data && data->philo)
		free(data->philo);
	if (data && data->death)
		free(data->death);
	if (data)
	{
		pthread_mutex_destroy(&data->print);
		pthread_mutex_destroy(&data->stop_mutex);
	}
	if (data && data->stop)
		free(data->stop);
	free(data);
}

void	*ft_calloc(size_t count, size_t size)
{
	void	*ptr;

	ptr = malloc(count * size);
	if (ptr == NULL)
		return (NULL);
	memset(ptr, 0, count * size);
	return (ptr);
}

void	mutexed_print(char *str, t_philo *philo)
{
	pthread_mutex_lock(philo->print);
	pthread_mutex_lock(philo->stop_mutex);
	if (*philo->stop == 1)
	{
		pthread_mutex_unlock(philo->stop_mutex);
		pthread_mutex_unlock(philo->print);
		return ;
	}
	pthread_mutex_unlock(philo->stop_mutex);
	printf("%ld %d %s", get_time_since_start(philo), philo->id, str);
	pthread_mutex_unlock(philo->print);
}
