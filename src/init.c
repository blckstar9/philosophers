/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aybelaou <aybelaou@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 19:11:05 by aybelaou          #+#    #+#             */
/*   Updated: 2025/07/05 14:23:52 by aybelaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file init.c
 * @brief Data structure initialization and argument parsing
 * 
 * Handles parsing command line arguments, initializing the main data
 * structure, and setting up philosopher structures with proper fork
 * assignments for the circular table arrangement.
 */

#include "../inc/philosopher.h"

/**
 * @brief Parse and validate command line arguments
 * @param data Pointer to data structure to populate
 * @param argc Number of arguments
 * @param argv Array of argument strings
 * @return true if all arguments are valid, false otherwise
 * 
 * Converts string arguments to integers and validates ranges.
 * Sets must_eat_count to -1 if not provided (unlimited meals).
 */
bool	parse_args(t_data *data, int argc, char **argv)
{
	bool	error;

	error = false;
	data->num_philos = ft_atoi_safe(argv[1], &error);
	data->time_to_die = ft_atoi_safe(argv[2], &error);
	data->time_to_eat = ft_atoi_safe(argv[3], &error);
	data->time_to_sleep = ft_atoi_safe(argv[4], &error);
	if (argc == 6)
		data->must_eat_count = ft_atoi_safe(argv[5], &error);
	else
		data->must_eat_count = -1;
	if (error || data->num_philos <= 0 || data->time_to_die <= 0
		|| data->time_to_eat <= 0 || data->time_to_sleep <= 0
		|| (argc == 6 && data->must_eat_count <= 0))
		return (false);
	return (true);
}

/**
 * @brief Initialize philosopher structures and assign forks
 * @param data Pointer to simulation data
 * @return true on success, false on memory allocation failure
 * 
 * Creates the philosopher array and assigns left/right forks in a
 * circular arrangement. Each philosopher gets adjacent forks, with
 * the last philosopher's right fork being the first fork (circular table).
 */
bool	init_philosophers(t_data *data)
{
	int	i;

	data->philos = malloc(sizeof(t_philo) * data->num_philos);
	if (!data->philos)
		return (false);
	i = 0;
	while (i < data->num_philos)
	{
		data->philos[i].id = i + 1;
		data->philos[i].meals_eaten = 0;
		data->philos[i].last_meal_time = 0;
		data->philos[i].data = data;
		data->philos[i].left_fork = &data->forks[i];
		data->philos[i].right_fork = &data->forks[(i + 1)
			% data->num_philos];
		i++;
	}
	return (true);
}

/**
 * @brief Main initialization function for simulation data
 * @param argc Number of command line arguments
 * @param argv Array of command line arguments
 * @return Pointer to initialized data structure, NULL on failure
 * 
 * Allocates and initializes the main data structure, parses arguments,
 * initializes mutexes, and sets up philosophers. Handles cleanup
 * on any initialization failure.
 */
t_data	*init_data(int argc, char **argv)
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	if (!data)
		return (NULL);
	memset(data, 0, sizeof(t_data));
	data->simulation_end = false;
	data->start_time = get_time();
	if (!parse_args(data, argc, argv))
	{
		free(data);
		return (NULL);
	}
	if (!init_mutexes(data))
	{
		free(data);
		return (NULL);
	}
	if (!init_philosophers(data))
	{
		free(data->forks);
		free(data);
		return (NULL);
	}
	return (data);
}
