/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iaktas <iaktas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 20:00:00 by iaktas            #+#    #+#             */
/*   Updated: 2025/09/01 12:05:37 by iaktas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"
#include <stdlib.h>

void clean_philos(t_philo *philos, int philo_count)
{
    int i;
    if (!philos)
        return;
    for (i = 0; i < philo_count; i++)
    {
        // Sadece thread başlatıldıysa join et
        if (philos[i].thread)
            pthread_join(philos[i].thread, NULL);
        pthread_mutex_destroy(&philos[i].am_i_alive_mutex);
    }
    free(philos);
}

void clean_guard(t_guard *guard, int philo_count)
{
	int i;
	if (guard)
	{
		if (guard->forks)
		{
			for (i = 0; i < philo_count; i++)
				pthread_mutex_destroy(&guard->forks[i]);
			free(guard->forks);
		}
		pthread_mutex_destroy(&guard->someone_died_mutex);
		pthread_mutex_destroy(&guard->can_i_speak);
	}
}

void clean_prison(t_prison *prison)
{
    if (prison)
    {
        clean_philos(prison->philos, prison->law.philo_count);
        clean_guard(&prison->guard, prison->law.philo_count);
    }
}
