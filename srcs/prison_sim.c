/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prison_sim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iaktas <iaktas@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 18:04:39 by iaktas            #+#    #+#             */
/*   Updated: 2025/08/31 19:13:43 by iaktas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	start_simulation(t_prison *prison)
{
	int	i;
	int	all_ate_enough;
	
	// Set start time
	prison->guard.start_time = get_time();
	
	// Create philosopher threads
	i = 0;
	while (i < prison->law.philo_count)
	{
		// last_meal_time değerini mutex ile koruyoruz
		pthread_mutex_lock(&prison->guard.is_die_mutex);
		prison->philos[i].last_meal_time = get_time();
		pthread_mutex_unlock(&prison->guard.is_die_mutex);
		
		if (pthread_create(&prison->philos[i].thread, NULL, 
						   philosopher_routine, &prison->philos[i]))
			return (1);
		// pthread_detach yerine join kullanacağız, bu yüzden burada detach yok
		i++;
	}

	// Main thread'de ölüm ve yemek kontrolü
	while (!prison->guard.someone_died)
	{
		i = 0;
		all_ate_enough = 1;
		
		// Check if all philosophers have eaten enough meals
		while (i < prison->law.philo_count)
		{
			if (!prison->philos[i].am_i_alive)
            {
                pthread_mutex_lock(&prison->guard.is_die_mutex);
                prison->guard.someone_died = 1;
                pthread_mutex_unlock(&prison->guard.is_die_mutex);
            }

            if (all_ate_enough && prison->philos[i].meals_eaten < prison->law.meals_required)
                all_ate_enough = 0;

			i++;
		}
		
		if (prison->law.meals_required != -1 && all_ate_enough)
		{
			pthread_mutex_lock(&prison->guard.is_die_mutex);
			prison->guard.someone_died = 1;
			pthread_mutex_unlock(&prison->guard.is_die_mutex);
			printf("%sAll philosophers have eaten enough meals!%s\n", GREEN, RESET);
			break;
		}
		
		safe_sleep(1000); // Check every 1ms
	}

	// Thread'leri join et - main process'in thread'lerin bitmesini beklemesini sağlar
	i = 0;
	while (i < prison->law.philo_count)
	{
		pthread_join(prison->philos[i].thread, NULL);
		i++;
	}
	
	return (0);
}
