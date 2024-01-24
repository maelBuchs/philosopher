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

int main(void)
{
	struct timeval g_start_time;


	gettimeofday(&g_start_time, NULL);
	printf("%ld seconds and %ld milliseconds\n", g_start_time.tv_sec, g_start_time.tv_usec);
	return 0;
}