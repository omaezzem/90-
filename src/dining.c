/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dining.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 09:49:59 by omaezzem          #+#    #+#             */
/*   Updated: 2025/04/07 03:17:12 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void single_ph(t_dining_table *table)
{
    if (!table || !table->forks || !table->philosophers)
        return;
    size_t start_time = get_current_time();
    pthread_mutex_lock(&table->forks[0]);
    printf("%zu -> %d : %s", get_current_time() - start_time,
           table->philosophers[0].id, MSG_FORK);
    usleep(table->time_to_die * 1000);
    printf("%zu -> %d : %s", get_current_time() - start_time,
           table->philosophers[0].id, MSG_DIED);
    pthread_mutex_unlock(&table->forks[0]);
}
void	ft_think(t_philosopher *philo)
{
	pthread_mutex_lock(&philo->table->print_lock);
	safe_print(philo, MSG_THK);
	pthread_mutex_unlock(&philo->table->print_lock);
}
void    ft_eat(t_philosopher *philo)
{
	pthread_mutex_lock(philo->left_fork);
	pthread_mutex_lock(&philo->table->print_lock);
	safe_print(philo, MSG_FORK);
	pthread_mutex_unlock(&philo->table->print_lock);
	pthread_mutex_lock(philo->right_fork);
	pthread_mutex_lock(&philo->table->print_lock);
	safe_print(philo, MSG_FORK);
	safe_print(philo, MSG_EAT);
	pthread_mutex_unlock(&philo->table->print_lock);
	pthread_mutex_lock(&philo->table->time_lock);
	philo->last_meal_time = get_current_time();
	pthread_mutex_unlock(&philo->table->time_lock);
	ft_ph_sleep(philo->table->time_to_eat, philo->table);
	pthread_mutex_lock(&philo->table->meal_lock);
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->table->meal_lock);
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
}

void    ft_sleep(t_philosopher *philo)
{
        safe_print(philo, MSG_SLP);
        ft_ph_sleep(philo->table->time_to_sleep, philo->table);
}


void    start_dining(t_philosopher *philo)
{
        if ((philo->id % 2 == 0) || philo->id == philo->table->philosopher_count)
                ft_ph_sleep(philo->table->time_to_eat / 2, philo->table);
        while (1)
        {
                if (ft_exit_dining(philo))
                        break ;
                ft_eat(philo);
                if (ft_exit_dining(philo))
                        break ;
                ft_sleep(philo);
                if (ft_exit_dining(philo))
                        break ;
                ft_think(philo);
        }
        return ;
}

int     bismillah(t_dining_table *table)
{
        pthread_t       hypervsr;
        int             i;

        if (table->philosopher_count == 1)
        {
                single_ph(table);
                return 1;
        }
        if (pthread_create(&hypervsr, NULL, (void *)hyper, table) != 0)
                return 0;
        i = 0;
        while (i < table->philosopher_count)
        {
                if (pthread_create(&table->philosophers[i].thread, NULL, (void *)start_dining,
                &table->philosophers[i]) != 0)
                        return 0;
                i++;
        }
        i = 0;
        while (i < table->philosopher_count)
        {
                if ((pthread_join(table->philosophers[i].thread, NULL)) != 0)
                        return 0;
                i++;
        }
        if (pthread_join(hypervsr, NULL) != 0)
                return 0;
        return 1; 
}
