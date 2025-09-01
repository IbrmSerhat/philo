/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iaktas <iaktas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 20:15:00 by iaktas            #+#    #+#             */
/*   Updated: 2025/09/01 15:12:56 by iaktas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static int	ft_atoi(const char *str)
{
	int	i;
	int	sign;
	int	ret;

	i = 0;
	sign = 1;
	ret = 0;
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n'
		|| str[i] == '\r' || str[i] == '\v' || str[i] == '\f')
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
		{
			sign *= -1;
		}
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		ret = (str[i] - '0') + (ret * 10);
		i++;
	}
	return (ret * sign);
}

int	init_law(t_law *law, int argc, char **argv)
{
	if (argc < 5 || argc > 6)
		return (1);
	law->philo_count = ft_atoi(argv[1]);
	law->time_to_die = ft_atoi(argv[2]);
	law->time_to_eat = ft_atoi(argv[3]);
	law->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		law->meals_required = ft_atoi(argv[5]);
	else
		law->meals_required = -1;
	if (law->philo_count <= 0 || law->philo_count > PHILO_MAX_COUNT
		|| law->time_to_die <= 0 || law->time_to_eat <= 0
		|| law->time_to_sleep <= 0 || (argc == 6 && law->meals_required <= 0))
		return (1);
	return (0);
}

int	init_guard(t_guard *guard, t_law *prison_law)
{
	int	i;

	guard->someone_died = 0;
	if (pthread_mutex_init(&guard->someone_died_mutex, NULL))
		return (1);
	if (pthread_mutex_init(&guard->can_i_speak, NULL))
		return (1);
	guard->start_time = 0;
	guard->law = prison_law;
	guard->forks = malloc(sizeof(pthread_mutex_t) * guard->law->philo_count);
	if (!guard->forks)
		return (1);
	i = 0;
	while (i < guard->law->philo_count)
	{
		if (pthread_mutex_init(&guard->forks[i], NULL))
			return (1);
		i++;
	}
	return (0);
}

int	init_philos(t_philo **philos, t_law *law, t_guard *guard)
{
	int	i;

	i = 0;
	*philos = malloc(sizeof(t_philo) * law->philo_count);
	if (!(*philos))
		return (1);
	while (i < law->philo_count)
	{
		(*philos)[i].id = i + 1;
		(*philos)[i].left_fork = i;
		(*philos)[i].right_fork = (i + 1) % law->philo_count;
		(*philos)[i].last_meal_time = 0;
		(*philos)[i].meals_eaten = 0;
		(*philos)[i].am_i_alive = 1;
		(*philos)[i].ask_guard = guard;
		if (pthread_mutex_init(&(*philos)[i].am_i_alive_mutex, NULL))
			return (1);
		i++;
	}
	return (0);
}

int	init_prison(t_prison *prison, int argc, char **argv)
{
	if (init_law(&prison->law, argc, argv))
	{
		printf("Error: Invalid arguments\n");
		return (1);
	}
	if (init_guard(&prison->guard, &prison->law))
	{
		printf("Error: Could not initialize guard\n");
		return (1);
	}
	if (init_philos(&prison->philos, &prison->law, &prison->guard))
	{
		printf("Error: Could not initialize philosophers\n");
		return (1);
	}
	return (0);
}
