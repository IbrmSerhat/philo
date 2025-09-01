/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iaktas <iaktas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 18:07:21 by iaktas            #+#    #+#             */
/*   Updated: 2025/09/01 13:07:43 by iaktas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	print_message(t_philo *philo, char *msg, int id, int am_i_alive)
{
	
	
	size_t	current_time;
	char	*philo_color;
	// Başlangıçta 0, ilk ölümden sonra 1 olacak. 0: print serbest, 1: sadece ölüm mesajı basılır
	static int print_blocked;

	pthread_mutex_lock(&philo->ask_guard->can_i_speak);

	// İlk ölümde print_blocked = 1 yapılır
	if (!am_i_alive)
	{
		// write(STDERR_FILENO, "++++++++++++++++++++++++++++++++\n", 33);
		print_blocked = 1;
	}

	// Eğer print_blocked aktifse ve hala yaşayan bir filozofsa, mesaj basma
	if (print_blocked == 1 && am_i_alive)
	{
		pthread_mutex_unlock(&philo->ask_guard->can_i_speak);
		return;
	}

	current_time = get_time() - philo->ask_guard->start_time;

	if (!am_i_alive)
		philo_color = RED;
	else
		philo_color = COLORS[(id - 1) % 10];

	if (!print_blocked)
		printf("%s%zu %d %s%s\n", philo_color, current_time, philo->id, msg, RESET);

	pthread_mutex_unlock(&philo->ask_guard->can_i_speak);
}