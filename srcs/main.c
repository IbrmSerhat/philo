/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iaktas <iaktas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 18:10:05 by iaktas            #+#    #+#             */
/*   Updated: 2025/09/01 12:29:07 by iaktas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/philo.h"

int main(int argc, char **argv)
{
    t_prison prison;

    //printf("[DEBUG] main: program started\n");
    if (init_prison(&prison, argc, argv))
    {
        //printf("[DEBUG] main: init_prison failed\n");
        return (1);
    }

    //printf("[DEBUG] main: init_prison success\n");
    if (start_simulation(&prison))
    {
        //printf("Error: Failed to start simulation\n");
        clean_prison(&prison);
        return (1);
    }

    //printf("[DEBUG] main: simulation finished\n");
    // Program biterken tüm kaynakları temizle
    clean_prison(&prison);
    //printf("[DEBUG] main: resources cleaned\n");
    return (0);
}