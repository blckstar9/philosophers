/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aybelaou <aybelaou@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 19:11:16 by aybelaou          #+#    #+#             */
/*   Updated: 2025/07/05 14:32:18 by aybelaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file utils.c
 * @brief Core utility functions for timing and input validation
 * 
 * Provides essential utilities: time management, precise sleep,
 * simulation state checking, and command line input validation.
 */

#include "../inc/philosopher.h"

/**
 * @brief Get current time in milliseconds since epoch
 * @return Current timestamp in milliseconds
 * 
 * Uses gettimeofday() for microsecond precision, converts to milliseconds.
 * Used for all timing calculations in the simulation.
 */
long long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

/**
 * @brief Thread-safe check of simulation end status
 * @param data Pointer to simulation data
 * @return true if simulation should end, false otherwise
 * 
 * Protected by end_mutex to ensure atomic read of simulation_end flag.
 * Used by philosopher threads to check if they should terminate.
 */
bool	is_simulation_over(t_data *data)
{
	bool	result;

	pthread_mutex_lock(&data->end_mutex);
	result = data->simulation_end;
	pthread_mutex_unlock(&data->end_mutex);
	return (result);
}

/**
 * @brief High-precision sleep function
 * @param ms Sleep duration in milliseconds
 * 
 * Busy-waiting sleep with 50μs polling for precise timing control.
 * More accurate than standard sleep functions for short durations.
 */
void	precise_sleep(long long ms)
{
	long long	end_time;

	end_time = get_time() + ms;
	while (get_time() < end_time)
		usleep(50);
}

/**
 * @brief Validate command line arguments
 * @param argc Number of arguments
 * @param argv Array of argument strings
 * @return true if all arguments are valid positive integers, false otherwise
 * 
 * Validates that all arguments are positive integers using ft_atoi_safe.
 * Shows warning for large philosopher counts (>200) due to performance impact.
 */
bool	validate_input(int argc, char **argv)
{
	int		i;
	bool	error;

	i = 1;
	while (i < argc)
	{
		error = false;
		if (ft_atoi_safe(argv[i], &error) <= 0 || error)
		{
			ft_putstr(RED "Error: Argument must be a positive integer\n" RS);
			return (false);
		}
		i++;
	}
	if (ft_atoi_safe(argv[1], &error) > 200)
		ft_putstr(RED "Warning: Large nb of philos->performance issues?\n" RS);
	return (true);
}
