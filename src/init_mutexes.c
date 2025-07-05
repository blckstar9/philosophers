/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_mutexes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aybelaou <aybelaou@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 19:46:36 by aybelaou          #+#    #+#             */
/*   Updated: 2025/07/05 14:26:03 by aybelaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file init_mutexes.c
 * @brief Mutex initialization and cleanup with error handling
 * 
 * Provides robust mutex initialization with proper cleanup on failures.
 * Uses staged cleanup to ensure no resource leaks during initialization.
 */

#include "../inc/philosopher.h"

/**
 * @brief Clean up fork mutexes
 * @param data Pointer to simulation data
 * @param count Number of fork mutexes to destroy
 * 
 * Destroys the specified number of fork mutexes and frees the array.
 * Used during cleanup on initialization failure.
 */
static void	cleanup_fork_mutexes(t_data *data, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		pthread_mutex_destroy(&data->forks[i]);
		i++;
	}
	free(data->forks);
}

/**
 * @brief Clean up all mutexes based on initialization stage
 * @param data Pointer to simulation data
 * @param stage Initialization stage reached (determines what to cleanup)
 * 
 * Performs staged cleanup: stage 1=print, stage 2=print+end,
 * stage 3=print+end+meal. Always cleans up fork mutexes.
 */
static void	cleanup_all_mutexes(t_data *data, int stage)
{
	if (stage >= 3)
		pthread_mutex_destroy(&data->meal_mutex);
	if (stage >= 2)
		pthread_mutex_destroy(&data->end_mutex);
	if (stage >= 1)
		pthread_mutex_destroy(&data->print_mutex);
	cleanup_fork_mutexes(data, data->num_philos);
}

/**
 * @brief Initialize all fork mutexes
 * @param data Pointer to simulation data
 * @return true on success, false on failure
 * 
 * Allocates and initializes mutex array for forks. Each fork
 * is represented by a mutex to prevent simultaneous access.
 */
static bool	init_fork_mutexes(t_data *data)
{
	int	i;

	data->forks = malloc(sizeof(pthread_mutex_t) * data->num_philos);
	if (!data->forks)
		return (false);
	i = 0;
	while (i < data->num_philos)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
		{
			cleanup_fork_mutexes(data, i);
			return (false);
		}
		i++;
	}
	return (true);
}

/**
 * @brief Initialize all mutexes with error handling
 * @param data Pointer to simulation data
 * @return true if all mutexes initialized successfully, false otherwise
 * 
 * Initializes mutexes in order: forks, print, end, meal.
 * Provides proper cleanup on any failure using staged cleanup.
 */
bool	init_mutexes(t_data *data)
{
	if (!init_fork_mutexes(data))
		return (false);
	if (pthread_mutex_init(&data->print_mutex, NULL) != 0)
	{
		cleanup_all_mutexes(data, 0);
		return (false);
	}
	if (pthread_mutex_init(&data->end_mutex, NULL) != 0)
	{
		cleanup_all_mutexes(data, 1);
		return (false);
	}
	if (pthread_mutex_init(&data->meal_mutex, NULL) != 0)
	{
		cleanup_all_mutexes(data, 2);
		return (false);
	}
	return (true);
}
