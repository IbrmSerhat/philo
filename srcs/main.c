/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iaktas <iaktas@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 18:10:05 by iaktas            #+#    #+#             */
/*   Updated: 2025/08/30 20:32:14 by iaktas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/philo.h"

int main(int argc, char **argv)
{
    t_prison prison;

    if (init_prison(&prison, argc, argv))
    {
        return (1);
    }

    if (start_simulation(&prison))
    {
        printf("Error: Failed to start simulation\n");
        clean_prison(&prison);
        return (1);
    }

    // Program biterken tüm kaynakları temizle
    clean_prison(&prison);
    return (0);
}