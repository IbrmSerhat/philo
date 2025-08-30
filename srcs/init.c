/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iaktas <iaktas@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 20:15:00 by iaktas            #+#    #+#             */
/*   Updated: 2025/08/30 20:54:58 by iaktas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"
#include <stdlib.h> // malloc, atoi
#include <stdio.h>  // printf

int init_law(t_law *law, int argc, char **argv)
{
    if (argc < 5 || argc > 6)
        return (1);
    
    law->philo_count = atoi(argv[1]);
    law->time_to_die = atoi(argv[2]);
    law->time_to_eat = atoi(argv[3]);
    law->time_to_sleep = atoi(argv[4]);
    law->meals_required = (argc == 6) ? atoi(argv[5]) : -1;
    if (law->philo_count <= 0 || law->philo_count > PHILO_MAX_COUNT ||
        law->time_to_die <= 0 || law->time_to_eat <= 0 || law->time_to_sleep <= 0 ||
        (argc == 6 && law->meals_required <= 0))
        return (1);
        
    return (0);
}

int init_guard(t_guard *guard, t_law *prison_law)
{
    int i;
    
    guard->someone_died = 0;
    guard->start_time = 0;
    guard->law = prison_law;
    guard->forks = malloc(sizeof(pthread_mutex_t) * guard->law->philo_count);
    if (!guard->forks)
        return (1);
    for (i = 0; i < guard->law->philo_count; i++)
    {
        if (pthread_mutex_init(&guard->forks[i], NULL))
            return (1);
    }
    if (pthread_mutex_init(&guard->is_die_mutex, NULL))
        return (1);
    
    return (0);
}

int init_philos(t_philo **philos, t_law *law, t_guard *guard)
{
    int i;

    *philos = malloc(sizeof(t_philo) * law->philo_count);
    if (!(*philos))
        return (1);
    for (i = 0; i < law->philo_count; i++)
    {
        (*philos)[i].id = i + 1;
        (*philos)[i].left_fork = i;
        (*philos)[i].right_fork = (i + 1) % law->philo_count;
        (*philos)[i].last_meal_time = 0;
        (*philos)[i].meals_eaten = 0;
        (*philos)[i].ask_guard = guard;
    }
    
    return (0);
}

int init_prison(t_prison *prison, int argc, char **argv)
{
    // Initialize law (rules)
    if (init_law(&prison->law, argc, argv))
    {
        printf("Error: Invalid arguments\n");
        return (1);
    }
    
    // Initialize guard
    if (init_guard(&prison->guard, &prison->law))
    {
        printf("Error: Could not initialize guard\n");
        return (1);
    }
    
    // Initialize philosophers
    if (init_philos(&prison->philos, &prison->law, &prison->guard))
    {
        printf("Error: Could not initialize philosophers\n");
        return (1);
    }
    
    return (0);
}
