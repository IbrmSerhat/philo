/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iaktas <iaktas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 21:10:00 by iaktas            #+#    #+#             */
/*   Updated: 2025/09/01 14:59:30 by iaktas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"
#include <string.h>

int	philo_eat(t_philo *philo)
{
	int	meals_eaten;
	int	meals_required;

	if (single_fork(philo))
		return (1);
	if (multi_fork(philo))
		return (1);
	philo->last_meal_time = get_time();
	print_message(philo, "is eating", philo->id, philo->am_i_alive);
	meals_eaten = ++philo->meals_eaten;
	meals_required = philo->ask_guard->law->meals_required;
	a_questionable_sleep(philo, philo->ask_guard->law->time_to_eat);
	pthread_mutex_unlock(&philo->ask_guard->forks[philo->left_fork]);
	pthread_mutex_unlock(&philo->ask_guard->forks[philo->right_fork]);
	if (meals_required != -1 && meals_eaten >= meals_required)
	{
		pthread_mutex_lock(&philo->am_i_alive_mutex);
		philo->am_i_alive = 2;
		pthread_mutex_unlock(&philo->am_i_alive_mutex);
		return (1);
	}
	return (0);
}

void	philo_sleep(t_philo *philo)
{
	print_message(philo, "is sleeping", philo->id, philo->am_i_alive);
	a_questionable_sleep(philo, philo->ask_guard->law->time_to_sleep);
}

void	philo_think(t_philo *philo)
{
	print_message(philo, "is thinking", philo->id, philo->am_i_alive);
}

void	*philosopher_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		safe_sleep(1000);
	while (1)
	{
		if (philo_eat(philo))
			break ;
		philo_sleep(philo);
		philo_think(philo);
	}
	return (NULL);
}
