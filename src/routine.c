/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aybelaou <aybelaou@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 19:11:13 by aybelaou          #+#    #+#             */
/*   Updated: 2025/07/05 14:41:06 by aybelaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file routine.c
 * @brief Philosopher behavior and action implementations
 * 
 * This file contains the core philosopher actions: taking forks,
 * eating, and the main philosopher routine that coordinates the
 * eat-sleep-think cycle.
 */

#include "../inc/philosopher.h"

/**
 * @brief Acquire both forks using deadlock prevention algorithm
 * @param philo Pointer to the philosopher structure
 * 
 * Implements even/odd fork ordering to prevent circular dependencies:
 * - Even philosophers: left fork first, then right fork
 * - Odd philosophers: right fork first, then left fork
 * 
 * This ordering ensures no deadlock can occur even with all
 * philosophers trying to eat simultaneously.
 */
void	take_forks(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->left_fork);
		ft_print_status(philo, "has taken a fork", YL);
		pthread_mutex_lock(philo->right_fork);
		ft_print_status(philo, "has taken a fork", YL);
	}
	else
	{
		pthread_mutex_lock(philo->right_fork);
		ft_print_status(philo, "has taken a fork", YL);
		pthread_mutex_lock(philo->left_fork);
		ft_print_status(philo, "has taken a fork", YL);
	}
}

/**
 * @brief Execute the eating action
 * @param philo Pointer to the philosopher structure
 * 
 * Complete eating sequence:
 * 1. Acquire both forks (blocking operation)
 * 2. Check if simulation ended (early exit if needed)
 * 3. Print eating status
 * 4. Update meal timing and count (thread-safe)
 * 5. Sleep for eating duration
 * 6. Release both forks
 * 
 * The meal timing is recorded when eating starts, not when
 * forks are acquired, for accurate death detection.
 */
void	eat(t_philo *philo)
{
	take_forks(philo);
	if (is_simulation_over(philo->data))
	{
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
		return ;
	}
	ft_print_status(philo, "is eating", GR);
	pthread_mutex_lock(&philo->data->meal_mutex);
	philo->last_meal_time = get_time() - philo->data->start_time;
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->data->meal_mutex);
	precise_sleep(philo->data->time_to_eat);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

/**
 * @brief Main philosopher thread routine
 * @param arg Pointer to philosopher structure (cast from void*)
 * @return NULL when thread terminates
 * 
 * Implements the complete philosopher lifecycle:
 * 
 * 1. **Desynchronization**: Even philosophers delay 1ms to spread
 *    resource usage and prevent simultaneous fork grabbing
 * 
 * 2. **Main Loop**: Continues until simulation ends or required
 *    meals are consumed:
 *    - Eat (acquire forks, eat, release forks)
 *    - Check meal requirement completion
 *    - Sleep for specified duration  
 *    - Think (brief pause before next cycle)
 * 
 * 3. **Optimization**: 1ms delay during thinking phase prevents
 *    excessive CPU usage in tight timing scenarios
 * 
 * The routine is designed for optimal resource utilization while
 * maintaining realistic philosopher behavior patterns.
 */
void	*philosopher_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		usleep(1000);
	while (!is_simulation_over(philo->data))
	{
		eat(philo);
		if (philo->data->must_eat_count != -1)
		{
			pthread_mutex_lock(&philo->data->meal_mutex);
			if (philo->meals_eaten >= philo->data->must_eat_count)
			{
				pthread_mutex_unlock(&philo->data->meal_mutex);
				break ;
			}
			pthread_mutex_unlock(&philo->data->meal_mutex);
		}
		ft_print_status(philo, "is sleeping", MG);
		precise_sleep(philo->data->time_to_sleep);
		ft_print_status(philo, "is thinking", CY);
		if (philo->data->num_philos > 1)
			usleep(1000);
	}
	return (NULL);
}
