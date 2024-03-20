/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbuchs <mael@buchs.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 10:03:37 by mbuchs            #+#    #+#             */
/*   Updated: 2024/01/24 10:03:37 by mbuchs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void *hello_philo(void *p)
{
	t_philo *philo = (t_philo *)p;
	if(philo->id % 2 == 0)
		usleep(100000);
	while (1)
	{
		pthread_mutex_lock(&philo->forks[philo->id]);
			pthread_mutex_lock(philo->print);
				philo->forks_status[philo->id] = 1;
				printf("thread num %d a pris la fourchette de %d\n", philo->id, philo->id);
			pthread_mutex_unlock(philo->print);
			pthread_mutex_lock(philo->print);
				pthread_mutex_lock(&philo->forks[(philo->id + 1) % 10]);
		philo->forks_status[(philo->id + 1) % 10] = 1;
		printf("thread num %d a pris la fourchette de %d\n", philo->id, (philo->id + 1) % 10);
		pthread_mutex_unlock(philo->print);
		pthread_mutex_lock(philo->print);
		printf("thread num %d a MANGED la fourchette de %d et de %d\n", philo->id, philo->id, (philo->id + 1) % 10);
		philo->forks_status[philo->id] = 1;
		philo->forks_status[(philo->id + 1) % 10] = 0;
		pthread_mutex_unlock(&philo->forks[(philo->id + 1) % 10]);
		philo->forks_status[philo->id] = 0;
		pthread_mutex_unlock(&philo->forks[philo->id]);
		pthread_mutex_unlock(philo->print);

		usleep(100000);
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

int main(void)
{
	long int i = 0;
	pthread_mutex_t *forks = ft_calloc(sizeof(pthread_mutex_t), 10);
	pthread_t *threads = ft_calloc(sizeof(pthread_t), 10);
	t_philo *philo = ft_calloc(sizeof(t_philo), 10);
	t_data *data = ft_calloc(sizeof(t_data),1);
	pthread_mutex_t *print = ft_calloc(sizeof(pthread_mutex_t),1);
	int *forks_status = ft_calloc(sizeof(int),	 10);

	while (i < 10)
	{
		philo[i].data = *data;
		philo[i].id = i;
		philo[i].forks = forks;
		philo[i].print = print;
		philo[i].forks_status = forks_status;
		i++;
	}
	i = 0;
	while(i < 10)
	{
		pthread_create(&threads[i], NULL, hello_philo, &philo[i]);
		i++;
	}
	i = 0;
	while(i < 10)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
	return 0;
}