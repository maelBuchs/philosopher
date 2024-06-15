/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbuchs <mbuchs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 22:01:19 by mbuchs            #+#    #+#             */
/*   Updated: 2024/06/15 22:48:36 by mbuchs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	check_args(int argc, char **argv)
{
	int	i;
	int	j;

	i = 1;
	while (i < argc)
	{
		j = 0;
		while (argv[i][j])
		{
			if (argv[i][j] < '0' || argv[i][j] > '9')
				ft_exit("invalid argument", NULL, NULL, NULL);
			j++;
		}
		i++;
	}
}

void	solo_philo(char **argv)
{
	long	start_time;
	long	current_time;
	int		time_to_die;
	int		time_to_eat;

	if (ft_isallnum(argv[2]) && ft_isallnum(argv[3]) && ft_isallnum(argv[4]))
		ft_exit("invalid argument", NULL, NULL, NULL);
	time_to_die = ft_atoi(argv[2]);
	time_to_eat = ft_atoi(argv[3]);
	start_time = get_time();
	current_time = get_time();
	printf("%ld 0 has taken a fork\n", current_time - start_time);
	usleep(time_to_die * 1000);
	printf("%ld 0 is dead\n", get_time() - start_time);
	ft_exit(NULL, NULL, NULL, NULL);
}

int	parsing(int ac, char **av, t_data **data, pthread_mutex_t **f)
{
	if (ac != 5 && ac != 6)
		ft_exit("invalid number of arguments", NULL, NULL, NULL);
	if (ft_atoi(av[1]) == 1)
		free((*data));
	if (ft_atoi(av[1]) == 1)
		solo_philo(av);
	check_args(ac, av);
	(*data)->nb_philo = ft_atoi(av[1]);
	if ((*data)->nb_philo < 1 || (*data)->nb_philo > 200)
		ft_exit("invalid number of philosophers", NULL, NULL, NULL);
	(*data)->stop = ft_calloc(1, sizeof(int));
	pthread_mutex_init(&(*data)->stop_mutex, NULL);
	(*data)->nb_must_eat = -1;
	if (ac == 6)
		(*data)->nb_must_eat = ft_atoi(av[5]);
	(*data)->philo = ft_calloc((*data)->nb_philo, sizeof(t_philo));
	(*data)->death = ft_calloc((*data)->nb_philo, sizeof(int));
	pthread_mutex_init(&(*data)->print, NULL);
	init_all(data, f);
	return (0);
}

void	free_all_thread(pthread_t *threads, int i, t_data *data)
{
	while (i >= 0)
	{
		pthread_join(threads[i], NULL);
		i--;
	}
	ft_exit("error creating thread\n", NULL, threads, data);
}

int	ft_isallnum(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (1);
		i++;
	}
	return (0);
}
