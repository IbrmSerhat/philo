/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iaktas <iaktas@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 20:00:00 by iaktas            #+#    #+#             */
/*   Updated: 2025/08/30 20:56:20 by iaktas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"
#include <stdlib.h>

void clean_philos(t_philo *philos, int philo_count)
{
    int i;

    if (philos)
    {
        for (i = 0; i < philo_count; i++)
        {
            if (philos[i].thread)
                pthread_detach(philos[i].thread);
        }
        free(philos);
    }
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
        pthread_mutex_destroy(&guard->is_die_mutex);
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
