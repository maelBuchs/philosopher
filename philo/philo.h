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

# include <sys/time.h>
# include <stdlib.h>
# include <pthread.h>
# include <string.h>
# include <unistd.h>
# include <stdio.h>

typedef struct s_philo {
	int				id;
	int				nb_meal;
	long			last_meal;
	long			start_time;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				nb_must_eat;
	pthread_mutex_t	*l_fork;
	pthread_mutex_t	*r_fork;
	pthread_mutex_t	*print;
	pthread_mutex_t	*stop_mutex;
	int				*stop;
	pthread_mutex_t	time_mutex;
	pthread_mutex_t	*start_mutex;
	pthread_mutex_t	*ready_count_mutex;
	int				*ready_count;
	int				nb_philo;
}	t_philo;

typedef struct s_data {
	int				nb_philo;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				nb_must_eat;
	int				*stop;
	pthread_mutex_t	stop_mutex;
	pthread_mutex_t	print;
	t_philo			*philo;
	int				*death;
	pthread_mutex_t	start_mutex;
	pthread_mutex_t	ready_count_mutex;
	int				ready_count;
}	t_data;

void		free_all(pthread_mutex_t *forks, pthread_t *threads, t_data *data);
void		ft_exit(char *str, pthread_mutex_t *forks,
				pthread_t *threads, t_data *data);
void		solo_philo(char **argv);
int			ft_atoi(const char *str);
int			ft_isallnum(char *str);
void		free_all(pthread_mutex_t *forks, pthread_t *threads, t_data *data);
int			init_all(t_data **data, pthread_mutex_t **forks);
void		*ft_calloc(size_t count, size_t size);
int			parsing(int ac, char **av, t_data **data, pthread_mutex_t **f);
long int	get_time(void);
void		free_all_thread(pthread_t *threads, int i, t_data *data);
int			philosophers(t_data *data, pthread_mutex_t *forks);
void		*monitoring(void *p);
void		*hello_philo(void *p);
long int	get_time_since_start(t_philo *philo);
int			philo_sleep_and_think(t_philo *philo);
void		mutexed_print(char *str, t_philo *philo);
void		mutex_get_time(t_philo *philo);
void		destroy_mutexes(t_data *data, pthread_mutex_t *forks);
int			create_and_join_threads(t_data *data, pthread_t *threads);
#endif
