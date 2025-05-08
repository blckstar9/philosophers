/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aybelaou <aybelaou@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 19:11:13 by aybelaou          #+#    #+#             */
/*   Updated: 2025/05/08 19:18:33 by aybelaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosopher.h"

// Take forks for eating
void	take_forks(t_philo *philo)
{
    // Even philosophers take left fork first, odd take right fork first
    // This prevents deadlock
    if (philo->id % 2 == 0)
    {
        pthread_mutex_lock(philo->left_fork);
        print_status(philo, "has taken a fork", YL);
        pthread_mutex_lock(philo->right_fork);
        print_status(philo, "has taken a fork", YL);
    }
    else
    {
        pthread_mutex_lock(philo->right_fork);
        print_status(philo, "has taken a fork", YL);
        pthread_mutex_lock(philo->left_fork);
        print_status(philo, "has taken a fork", YL);
    }
}

// Eat action
void	eat(t_philo *philo)
{
    take_forks(philo);
    
    print_status(philo, "is eating", GR);
    philo->last_meal_time = get_time();
    philo->meals_eaten++;
    
    precise_sleep(philo->data->time_to_eat);
    
    // Release forks
    pthread_mutex_unlock(philo->left_fork);
    pthread_mutex_unlock(philo->right_fork);
}

// Main philosopher routine
void	*philosopher_routine(void *arg)
{
    t_philo	*philo;
    
    philo = (t_philo *)arg;
    
    // Stagger philosopher start times to prevent immediate deadlock
    if (philo->id % 2 == 0)
        usleep(1000);
    
    // Record first meal time
    philo->last_meal_time = get_time();
    
    while (!is_simulation_over(philo->data))
    {
        // Eating
        eat(philo);
        
        // Check if reached required meals
        if (philo->data->must_eat_count != -1 && 
            philo->meals_eaten >= philo->data->must_eat_count)
            break;
        
        // Sleeping
        print_status(philo, "is sleeping", MG);
        precise_sleep(philo->data->time_to_sleep);
        
        // Thinking
        print_status(philo, "is thinking", CY);
    }
    
    return (NULL);
}