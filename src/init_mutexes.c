/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_mutexes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aybelaou <aybelaou@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 19:46:36 by aybelaou          #+#    #+#             */
/*   Updated: 2025/06/23 19:46:45 by aybelaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosopher.h"

// Helper function to clean up fork mutexes
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

// Helper function to clean up all mutexes in reverse order
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

// Initialize fork mutexes
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

// Initialize mutexes
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
