/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iaktas <iaktas@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 21:10:00 by iaktas            #+#    #+#             */
/*   Updated: 2025/08/31 19:01:07 by iaktas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"
#include <string.h>

int could_i_be_dead(t_philo *philo)
{
	size_t	current_time;
	size_t	time_since_last_meal;

	current_time = get_time();
	
	time_since_last_meal = current_time - philo->last_meal_time;

	if (time_since_last_meal > (size_t)philo->ask_guard->law->time_to_die)
	{
		print_message(philo, "died", philo->id, philo->am_i_alive);
		philo->am_i_alive = 0;
		return (1);
	}
	return (0);
}

void	a_questionable_sleep(t_philo *philo, int sleep_time)
{
	size_t	end_time;
	size_t	current_time;

	current_time = get_time();
	end_time = current_time + sleep_time;
	
	while (current_time < end_time)
    {
        pthread_mutex_lock(&philo->ask_guard->is_die_mutex);
        int someone_died = philo->ask_guard->someone_died;
        pthread_mutex_unlock(&philo->ask_guard->is_die_mutex);
        
        if (someone_died)
            break;
            
        safe_sleep(500);
        current_time = get_time();
    }
}

int	philo_eat(t_philo *philo)
{
	if (philo->left_fork < philo->right_fork)
	{
		pthread_mutex_lock(&philo->ask_guard->forks[philo->left_fork]);
		print_message(philo, "has taken a fork", philo->id, philo->am_i_alive);
		pthread_mutex_lock(&philo->ask_guard->forks[philo->right_fork]);
		print_message(philo, "has taken a fork", philo->id, philo->am_i_alive);
	}
	else
	{
		pthread_mutex_lock(&philo->ask_guard->forks[philo->right_fork]);
		print_message(philo, "has taken a fork", philo->id, philo->am_i_alive);
		pthread_mutex_lock(&philo->ask_guard->forks[philo->left_fork]);
		print_message(philo, "has taken a fork", philo->id, philo->am_i_alive);
	}

	// Update last meal time with mutex protection
	if(could_i_be_dead(philo))
	{
		pthread_mutex_unlock(&philo->ask_guard->forks[philo->left_fork]);
		pthread_mutex_unlock(&philo->ask_guard->forks[philo->right_fork]);
		return (1);
	}
	philo->last_meal_time = get_time();
	print_message(philo, "is eating", philo->id, philo->am_i_alive);
	int meals_eaten = ++philo->meals_eaten;
	int meals_required = philo->ask_guard->law->meals_required;

	a_questionable_sleep(philo, philo->ask_guard->law->time_to_eat);
	
	// Unlock forks
	pthread_mutex_unlock(&philo->ask_guard->forks[philo->left_fork]);
	pthread_mutex_unlock(&philo->ask_guard->forks[philo->right_fork]);

	if (meals_required != -1 && meals_eaten >= meals_required)
		return (1); // Exit if enough meals eaten
	
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
	size_t	current_time;
	size_t	time_since_last_meal;

	philo = (t_philo *)arg;
	
	// last_meal_time değerini mutex ile koruyoruz
	pthread_mutex_lock(&philo->ask_guard->is_die_mutex);
	philo->last_meal_time = get_time(); // Set initial meal time
	pthread_mutex_unlock(&philo->ask_guard->is_die_mutex);

	// If only one philosopher, they will die (only one fork available)
	if (philo->ask_guard->law->philo_count == 1)
	{
		print_message(philo, "has taken a fork", philo->id, philo->am_i_alive);
		a_questionable_sleep(philo, philo->ask_guard->law->time_to_die);
		print_message(philo, "died", philo->id, philo->am_i_alive);
		return (NULL);
	}

	// Even philosophers start a bit later to avoid deadlocks
	if (philo->id % 2 == 0)
		safe_sleep(1000); // 1ms delay

	// Main loop
	while (1)
	{
		// Check if philosopher is starving before any action
		current_time = get_time();
		
		pthread_mutex_lock(&philo->ask_guard->can_i_eat[philo->id - 1]);
		time_since_last_meal = current_time - philo->last_meal_time;
		
		if (could_i_be_dead(philo))
		{
			pthread_mutex_unlock(&philo->ask_guard->can_i_eat[philo->id - 1]);
			break;
		}
		
		if (time_since_last_meal > (size_t)philo->ask_guard->law->time_to_die)
		{
			pthread_mutex_unlock(&philo->ask_guard->can_i_eat[philo->id - 1]);
			print_message(philo, "died", philo->id, philo->am_i_alive);
			break;
		}
		pthread_mutex_unlock(&philo->ask_guard->can_i_eat[philo->id - 1]);
		
		if (philo_eat(philo))
			break;
		
		philo_sleep(philo);
		philo_think(philo);
	}

	return (NULL);
}

