/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iaktas <iaktas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 13:45:19 by iaktas            #+#    #+#             */
/*   Updated: 2025/09/01 14:00:05 by iaktas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stddef.h>  // offsetof
# include <sys/time.h> // gettimeofday
# include <stdlib.h> // malloc, free
# include <stdio.h>  // printf
# include <unistd.h> // usleep

// Renkler
# define RESET			"\033[0m"
# define RED			"\033[31m"
# define GREEN			"\033[32m"
# define YELLOW			"\033[33m"
# define BLUE			"\033[34m"
# define MAGENTA		"\033[35m"
# define CYAN			"\033[36m"
# define WHITE			"\033[37m"
# define BRIGHT_BLACK	"\033[90m"
# define BRIGHT_RED		"\033[91m"

// Sabitler
# define PHILO_MAX_COUNT 200

typedef struct s_philo		t_philo;
typedef struct s_guard		t_guard;
typedef struct s_law		t_law;
typedef struct s_prison		t_prison;

// Init functions
int		init_law(t_law *law, int argc, char **argv);
int		init_guard(t_guard *guard, t_law *law);
int		init_philos(t_philo **philos, t_law *law, t_guard *guard);
int		init_prison(t_prison *prison, int argc, char **argv);

// Cleanup functions
void	clean_philos(t_philo *philos, int philo_count);
void	clean_guard(t_guard *guard, int philo_count);
void	clean_prison(t_prison *prison);

// Philosopher functions
void	*philosopher_routine(void *arg);
int		philo_eat(t_philo *philo);
void	philo_sleep(t_philo *philo);
void	philo_think(t_philo *philo);

size_t	get_time(void);
void	print_message(t_philo *philo, char *msg, int id, int die);
void	precise_sleep(t_philo *philo, int sleep_time);
void	safe_sleep(int microseconds);

// prison_sim.c
int		start_simulation(t_prison *prison);

struct s_philo
{
	pthread_t		thread;
	int				id;
	int				left_fork;
	int				right_fork;
	size_t			last_meal_time;
	int				can_i_eat;
	int				meals_eaten;
	int				am_i_alive;
	pthread_mutex_t	am_i_alive_mutex;
	struct s_guard	*ask_guard;
};

struct s_guard
{
	pthread_mutex_t	can_i_speak;
	int				someone_died;
	pthread_mutex_t	someone_died_mutex;
	size_t			start_time;
	pthread_mutex_t	*forks;
	t_law			*law;
};

struct s_law
{
	int				philo_count;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				meals_required;
};

struct s_prison
{
	t_law			law;
	t_philo			*philos;
	t_guard			guard;
};

#endif
