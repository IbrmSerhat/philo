/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iaktas <iaktas@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 21:10:00 by iaktas            #+#    #+#             */
/*   Updated: 2025/08/30 21:55:48 by iaktas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"
#include <string.h>

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

	chunk_size = 100; // Her seferinde 100 mikrosaniye uyuyacak
	remaining = microseconds;
	i = 0;

	// Uyku süresini 100'erlik parçalara böl
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

void	print_message(t_philo *philo, char *msg, int id, int die)
{
	size_t	current_time;
	char	*philo_color;

	pthread_mutex_lock(&philo->ask_guard->is_die_mutex);
	if (philo->ask_guard->someone_died && !die)
	{
		pthread_mutex_unlock(&philo->ask_guard->is_die_mutex);
		return;
	}
	
	current_time = get_time() - philo->ask_guard->start_time;
	
	// Eğer ölüm mesajı ise kırmızı kullan, yoksa ID'ye göre renk seç
	if (die)
		philo_color = RED;
	else
		philo_color = COLORS[(id - 1) % 10];
	
	printf("%s%zu %d %s%s\n", 
		philo_color, current_time, philo->id, msg, RESET);
	
	if (die)
		philo->ask_guard->someone_died = 1;
		
	pthread_mutex_unlock(&philo->ask_guard->is_die_mutex);
}

void	precise_sleep(t_philo *philo, int sleep_time)
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
	// Lock forks - always lock lower index fork first to prevent deadlocks
	if (philo->left_fork < philo->right_fork)
	{
		pthread_mutex_lock(&philo->ask_guard->forks[philo->left_fork]);
		print_message(philo, "has taken a fork", philo->id, 0);
		pthread_mutex_lock(&philo->ask_guard->forks[philo->right_fork]);
		print_message(philo, "has taken a fork", philo->id, 0);
	}
	else
	{
		pthread_mutex_lock(&philo->ask_guard->forks[philo->right_fork]);
		print_message(philo, "has taken a fork", philo->id, 0);
		pthread_mutex_lock(&philo->ask_guard->forks[philo->left_fork]);
		print_message(philo, "has taken a fork", philo->id, 0);
	}

	// Eating
	print_message(philo, "is eating", philo->id, 0);
	
	// last_meal_time ve meals_eaten değişkenlerini mutex ile koruyoruz
	pthread_mutex_lock(&philo->ask_guard->is_die_mutex);
	philo->last_meal_time = get_time();
	pthread_mutex_unlock(&philo->ask_guard->is_die_mutex);
	
	precise_sleep(philo, philo->ask_guard->law->time_to_eat);
	
	// meals_eaten değişkenini mutex ile koruyoruz
	pthread_mutex_lock(&philo->ask_guard->is_die_mutex);
	philo->meals_eaten++;
	int meals_required = philo->ask_guard->law->meals_required;
	int meals_eaten = philo->meals_eaten;
	pthread_mutex_unlock(&philo->ask_guard->is_die_mutex);

	// Unlock forks
	pthread_mutex_unlock(&philo->ask_guard->forks[philo->left_fork]);
	pthread_mutex_unlock(&philo->ask_guard->forks[philo->right_fork]);

	if (meals_required != -1 && meals_eaten >= meals_required)
		return (1); // Exit if enough meals eaten
	
	return (0);
}

void	philo_sleep(t_philo *philo)
{
	print_message(philo, "is sleeping", philo->id, 0);
	precise_sleep(philo, philo->ask_guard->law->time_to_sleep);
}

void	philo_think(t_philo *philo)
{
	print_message(philo, "is thinking", philo->id, 0);
}

int	check_starvation(t_philo *philo)
{
	size_t	current_time;
	size_t	time_since_last_meal;
	size_t	last_meal_time;

	current_time = get_time();
	
	// last_meal_time erişimini mutex ile koruyoruz
	pthread_mutex_lock(&philo->ask_guard->is_die_mutex);
	last_meal_time = philo->last_meal_time;
	pthread_mutex_unlock(&philo->ask_guard->is_die_mutex);
	
	time_since_last_meal = current_time - last_meal_time;

	if (time_since_last_meal > (size_t)philo->ask_guard->law->time_to_die)
	{
		print_message(philo, "died", philo->id, 1);
		return (1);
	}
	return (0);
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
		print_message(philo, "has taken a fork", philo->id, 0);
		precise_sleep(philo, philo->ask_guard->law->time_to_die);
		print_message(philo, "died", philo->id, 1);
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
		
		pthread_mutex_lock(&philo->ask_guard->is_die_mutex);
		time_since_last_meal = current_time - philo->last_meal_time;
		
		if (philo->ask_guard->someone_died)
		{
			pthread_mutex_unlock(&philo->ask_guard->is_die_mutex);
			break;
		}
		
		if (time_since_last_meal > (size_t)philo->ask_guard->law->time_to_die)
		{
			pthread_mutex_unlock(&philo->ask_guard->is_die_mutex);
			print_message(philo, "died", philo->id, 1);
			break;
		}
		pthread_mutex_unlock(&philo->ask_guard->is_die_mutex);
		
		if (philo_eat(philo))
			break;
		philo_sleep(philo);
		philo_think(philo);
	}

	return (NULL);
}

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

	// Main thread sadece yeterli yemek kontrolü yapıyor
	while (!prison->guard.someone_died)
	{
		i = 0;
		all_ate_enough = 1;
		
		// Check if all philosophers have eaten enough meals
		while (i < prison->law.philo_count)
		{
			pthread_mutex_lock(&prison->guard.is_die_mutex);
			if (prison->guard.someone_died)
			{
				pthread_mutex_unlock(&prison->guard.is_die_mutex);
				break;
			}
			
			// Check if philosopher has eaten enough meals
			if (prison->law.meals_required != -1 && 
				prison->philos[i].meals_eaten < prison->law.meals_required)
				all_ate_enough = 0;
			
			pthread_mutex_unlock(&prison->guard.is_die_mutex);
			i++;
		}
		
		// If all philosophers have eaten enough meals, end simulation
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
