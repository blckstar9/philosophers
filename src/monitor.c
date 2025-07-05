/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aybelaou <aybelaou@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 19:11:09 by aybelaou          #+#    #+#             */
/*   Updated: 2025/07/05 14:28:22 by aybelaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file monitor.c
 * @brief Death monitoring and meal completion checking
 * 
 * Implements the monitoring thread that continuously checks for
 * philosopher deaths and simulation completion conditions.
 */

#include "../inc/philosopher.h"

/**
 * @brief Check if all philosophers have eaten the required number of meals
 * @param data Pointer to simulation data
 * @return true if all have eaten enough, false otherwise
 * 
 * Thread-safe check of meal counts. Returns false immediately
 * if must_eat_count is -1 (unlimited meals scenario).
 */
bool	all_ate_enough(t_data *data)
{
	int	i;

	if (data->must_eat_count == -1)
		return (false);
	i = 0;
	while (i < data->num_philos)
	{
		pthread_mutex_lock(&data->meal_mutex);
		if (data->philos[i].meals_eaten < data->must_eat_count)
		{
			pthread_mutex_unlock(&data->meal_mutex);
			return (false);
		}
		pthread_mutex_unlock(&data->meal_mutex);
		i++;
	}
	return (true);
}

/**
 * @brief Safely set the simulation end flag
 * @param data Pointer to simulation data
 * @param value New value for simulation_end flag
 * 
 * Thread-safe setter for the global simulation end flag.
 * Protected by end_mutex to prevent race conditions.
 */
void	set_simulation_end(t_data *data, bool value)
{
	pthread_mutex_lock(&data->end_mutex);
	data->simulation_end = value;
	pthread_mutex_unlock(&data->end_mutex);
}

/**
 * @brief Main monitoring loop for death detection and completion
 * @param data Pointer to simulation data
 * 
 * Continuously monitors all philosophers for death conditions by
 * checking time since last meal. Also checks if all philosophers
 * have eaten the required number of meals. Runs until death or completion.
 * 
 * Monitoring frequency: 1ms (usleep(1000))
 */
void	monitor_philosophers(t_data *data)
{
	int			i;
	long long	current_time;
	long long	time_since_meal_start;

	while (1)
	{
		i = -1;
		while (++i < data->num_philos)
		{
			pthread_mutex_lock(&data->meal_mutex);
			current_time = get_time() - data->start_time;
			time_since_meal_start = current_time
				- data->philos[i].last_meal_time;
			if (time_since_meal_start >= data->time_to_die)
			{
				ft_print_status(&data->philos[i], "died", RED);
				pthread_mutex_unlock(&data->meal_mutex);
				return (set_simulation_end(data, true));
			}
			pthread_mutex_unlock(&data->meal_mutex);
		}
		if (all_ate_enough(data))
			return (set_simulation_end(data, true));
		usleep(1000);
	}
}
