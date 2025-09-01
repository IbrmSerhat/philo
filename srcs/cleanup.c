/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iaktas <iaktas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 20:00:00 by iaktas            #+#    #+#             */
/*   Updated: 2025/09/01 14:06:36 by iaktas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"
#include <stdlib.h>

void	clean_philos(t_philo *philos, int philo_count)
{
	int i = 0;
	if (!philos)
		return;
	while (i < philo_count)
	{
		pthread_join(philos[i].thread, NULL);
		pthread_mutex_destroy(&philos[i].am_i_alive_mutex);
		i++;
	}
	free(philos);
	philos = NULL;
}

void	clean_guard(t_guard *guard, int philo_count)
{
	int i = 0;
	if (!guard)
		return;
	if (guard->forks)
	{
		while (i < philo_count)
		{
			pthread_mutex_destroy(&guard->forks[i]);
			i++;
		}
		free(guard->forks);
		guard->forks = NULL;
	}
	pthread_mutex_destroy(&guard->someone_died_mutex);
	pthread_mutex_destroy(&guard->can_i_speak);
}

void	clean_prison(t_prison *prison)
{
	if (prison)
	{
		clean_philos(prison->philos, prison->law.philo_count);
		clean_guard(&prison->guard, prison->law.philo_count);
	}
}
