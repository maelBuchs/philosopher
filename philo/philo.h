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
    int             id;
    int             time_to_die;
    int             time_to_eat;
    int             time_to_sleep;
    int             nb_must_eat;
    int             nb_meal;
    long int        start_time;
    pthread_mutex_t *l_fork;
    pthread_mutex_t *r_fork;
    pthread_mutex_t *print;
    int             *forks_status;
    long int        last_meal;
    int             *death;
    int             *stop;
} t_philo;

typedef struct s_data
{
    int     nb_philo;
    t_philo *philo;
    int     *stop;
    int     *death;
    pthread_mutex_t *stop_mutex;
} t_data;

#endif
