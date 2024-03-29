/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbuchs <mael@buchs.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 10:01:16 by mbuchs            #+#    #+#             */
/*   Updated: 2024/01/24 10:01:16 by mbuchs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h> // DONT FORGET TO REMOVE
# include <sys/time.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
 typedef struct s_data
  {
	int		nb_philo;
	int		time_to_die;
	int		time_to_eat;
	int		time_to_sleep;
	int		nb_must_eat;

  }		t_data;

 typedef struct s_philo
 {
	int		id;
	struct s_data data;
	pthread_mutex_t *forks;
	pthread_mutex_t *print;
	int *forks_status;
 } t_philo;


#endif