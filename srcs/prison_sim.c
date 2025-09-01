/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prison_sim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iaktas <iaktas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 18:04:39 by iaktas            #+#    #+#             */
/*   Updated: 2025/09/01 12:51:50 by iaktas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	start_simulation(t_prison *prison)
{
	int	i;
	
	// Set start time
	prison->guard.start_time = get_time();
	
	// Create philosopher threads
	i = 0;
	while (i < prison->law.philo_count)
	{
		prison->philos[i].last_meal_time = get_time();
		
		if (pthread_create(&prison->philos[i].thread, NULL, 
						   philosopher_routine, &prison->philos[i]))
			return (1);
		i++;
	}

	// Main thread'de ölüm ve yemek kontrolü
	while (!prison->guard.someone_died)
	{
		i = 0;
		int all_finished = 1;
		
		while (i< prison->guard.law->philo_count)
		{
			pthread_mutex_lock(&prison->philos[i].am_i_alive_mutex);
			if (prison->philos[i].am_i_alive == 0)
			{
				pthread_mutex_lock(&prison->guard.someone_died_mutex);
				prison->guard.someone_died = 1;
				pthread_mutex_unlock(&prison->guard.someone_died_mutex);
			}
			if (prison->philos[i].am_i_alive != 2)
        		all_finished = 0;
			pthread_mutex_unlock(&prison->philos[i].am_i_alive_mutex);
			i++;
		}
		if (all_finished)
    		break;
		
		safe_sleep(1000); // Check every 1ms
	}


	
	return (0);
}
