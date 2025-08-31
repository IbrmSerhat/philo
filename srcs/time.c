/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iaktas <iaktas@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 18:06:34 by iaktas            #+#    #+#             */
/*   Updated: 2025/08/31 18:07:38 by iaktas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

size_t	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	safe_sleep(int microseconds)
{
	int	chunk_size;
	int	remaining;
	int	i;

	chunk_size = 100;
	remaining = microseconds;
	i = 0;
	while (remaining > 0)
	{
		if (remaining < chunk_size)
		{
			usleep(remaining);
			break;
		}
		usleep(chunk_size);
		remaining -= chunk_size;
		i++;
	}
}