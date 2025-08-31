/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iaktas <iaktas@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 18:07:21 by iaktas            #+#    #+#             */
/*   Updated: 2025/08/31 19:16:58 by iaktas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	print_message(t_philo *philo, char *msg, int id, int am_i_alive)
{
	size_t	current_time;
	char	*philo_color;
    static int can_i_print; // Static her oluştuğunda 0 gelir.

	pthread_mutex_lock(&philo->ask_guard->is_die_mutex);

    if (!am_i_alive)
    {
        can_i_print = 0; // İlk ölümden sonra print etmeyi durdur.
    }

	if (!philo->ask_guard->someone_died || !am_i_alive)
	{
		pthread_mutex_unlock(&philo->ask_guard->is_die_mutex);
		return;
	}
	current_time = get_time() - philo->ask_guard->start_time;

    // Renk kodlarını tanımla
	if (am_i_alive)
		philo_color = RED;
	else
		philo_color = COLORS[(id - 1) % 10];
	
    if (can_i_print)
    {
        printf("%s%zu %d %s%s\n", 
		    philo_color, current_time, philo->id, msg, RESET);
    }
	   
			
	pthread_mutex_unlock(&philo->ask_guard->is_die_mutex);
}