/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iaktas <iaktas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 18:07:21 by iaktas            #+#    #+#             */
/*   Updated: 2025/09/01 14:02:02 by iaktas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static char	*get_color(int idx)
{
	char *i;
	static char	*colors[10] = {
		RED, GREEN, YELLOW, BLUE, MAGENTA,
		CYAN, WHITE, BRIGHT_BLACK, BRIGHT_RED, RESET
	};
	
	i = colors[idx % 10];
	return (i);
}

void	print_message(t_philo *philo, char *msg, int id, int am_i_alive)
{
	size_t		current_time;
	char		*philo_color;
	static int	print_blocked;

	pthread_mutex_lock(&philo->ask_guard->can_i_speak);
	if (!am_i_alive)
		print_blocked = 1;
	if (print_blocked == 1 && am_i_alive)
	{
		pthread_mutex_unlock(&philo->ask_guard->can_i_speak);
		return;
	}
	current_time = get_time() - philo->ask_guard->start_time;
	if (!am_i_alive)
		philo_color = RED;
	else
		philo_color = get_color(id - 1);
	if (!print_blocked)
		printf("%s%zu %d %s%s\n", philo_color, current_time, philo->id, msg, RESET);
	pthread_mutex_unlock(&philo->ask_guard->can_i_speak);
}