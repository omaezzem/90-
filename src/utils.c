/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 03:12:00 by omaezzem          #+#    #+#             */
/*   Updated: 2025/04/07 03:20:01 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

int ft_ph_sleep(size_t time, t_dining_table *table)
{
    size_t	start;

    start = get_current_time();
    while ((get_current_time() - start) < time)
    {
        pthread_mutex_lock(&table->death_lock);
        if (table->die_flag)
        {
            pthread_mutex_unlock(&table->death_lock);
            return (0);
        }
        pthread_mutex_unlock(&table->death_lock);
        usleep(100);
    }
    return (0);
}

