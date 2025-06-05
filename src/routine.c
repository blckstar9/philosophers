/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aybelaou <aybelaou@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 19:11:13 by aybelaou          #+#    #+#             */
/*   Updated: 2025/06/05 23:04:14 by aybelaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosopher.h"

void	take_forks(t_philo *philo)
{
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

void	eat(t_philo *philo)
{
    take_forks(philo);
    if (is_simulation_over(philo->data))
    {
        pthread_mutex_unlock(philo->left_fork);
        pthread_mutex_unlock(philo->right_fork);
        return;
    }
    print_status(philo, "is eating", GR);
    pthread_mutex_lock(&philo->data->meal_mutex);
    philo->last_meal_time = get_time();  // Update AFTER printing "is eating"
    philo->meals_eaten++;
    pthread_mutex_unlock(&philo->data->meal_mutex);
    precise_sleep(philo->data->time_to_eat);
    pthread_mutex_unlock(philo->left_fork);
    pthread_mutex_unlock(philo->right_fork);
}

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
		print_status(philo, "is sleeping", MG);
		precise_sleep(philo->data->time_to_sleep);
		print_status(philo, "is thinking", CY);
	}
	return (NULL);
}
