/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbuchs <mbuchs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 09:24:18 by mbuchs            #+#    #+#             */
/*   Updated: 2024/06/08 09:24:18 by mbuchs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h> // DONT FORGET TO REMOVE
# include <sys/time.h>
# include <stdlib.h>
# include <pthread.h>
# include <string.h>
# include <unistd.h>

typedef struct s_philo
{
	int				id;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				nb_must_eat;
	int				nb_meal;
	long int		start_time;
	pthread_mutex_t	*l_fork;
	pthread_mutex_t	*r_fork;
	pthread_mutex_t	*print;
	long int		last_meal;
	int				*death;
	int				*stop;
	pthread_mutex_t	*stop_mutex;
	pthread_mutex_t	time_mutex;
}	t_philo;

typedef struct s_data
{
	int				nb_philo;
	t_philo			*philo;
	int				*stop;
	int				nb_must_eat;
	int				*death;
	pthread_mutex_t	stop_mutex;
	pthread_mutex_t	print;
}	t_data;

#endif
