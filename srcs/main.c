/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iaktas <iaktas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 18:10:05 by iaktas            #+#    #+#             */
/*   Updated: 2025/09/01 13:39:01 by iaktas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	main(int argc, char **argv)
{
	t_prison	prison;

	if (init_prison(&prison, argc, argv))
		return (1);
	if (start_simulation(&prison))
	{
		clean_prison(&prison);
		return (1);
	}
	clean_prison(&prison);
	return (0);
}
