/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aybelaou <aybelaou@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 18:46:18 by aybelaou          #+#    #+#             */
/*   Updated: 2025/07/05 14:22:16 by aybelaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file main.c
 * @brief Main program entry point and thread management
 * 
 * Contains the main function, thread creation/joining, and resource cleanup.
 * Handles program initialization, single philosopher edge case, and
 * coordinates the overall simulation lifecycle.
 */

#include "../inc/philosopher.h"

/**
 * @brief Clean up all allocated resources and destroy mutexes
 * @param data Pointer to simulation data structure
 * 
 * Safely destroys all mutexes and frees allocated memory.
 * Handles NULL pointer checks to prevent crashes during cleanup.
 */
void	clean_up(t_data *data)
{
	int	i;

	if (!data)
		return ;
	if (data->forks)
	{
		i = 0;
		while (i < data->num_philos)
		{
			pthread_mutex_destroy(&data->forks[i]);
			i++;
		}
		free(data->forks);
	}
	pthread_mutex_destroy(&data->print_mutex);
	pthread_mutex_destroy(&data->end_mutex);
	pthread_mutex_destroy(&data->meal_mutex);
	if (data->philos)
		free(data->philos);
	free(data);
}

/**
 * @brief Create and start all philosopher threads
 * @param data Pointer to simulation data
 * @return true if all threads created successfully, false otherwise
 * 
 * Initializes last meal times and creates pthread for each philosopher.
 * Uses philosopher_routine as the thread entry point.
 */
bool	create_philosophers(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		data->philos[i].last_meal_time = 0;
		i++;
	}
	i = 0;
	while (i < data->num_philos)
	{
		if (pthread_create(&data->philos[i].thread, NULL,
				philosopher_routine, &data->philos[i]) != 0)
			return (false);
		i++;
	}
	return (true);
}

/**
 * @brief Wait for all philosopher threads to complete
 * @param data Pointer to simulation data
 * 
 * Calls pthread_join on all philosopher threads to ensure
 * proper cleanup before program termination.
 */
void	join_philosophers(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		pthread_join(data->philos[i].thread, NULL);
		i++;
	}
}

/**
 * @brief Main program entry point
 * @param argc Number of command line arguments
 * @param argv Array of command line argument strings
 * @return Exit code (0 on success, 1 on error)
 * 
 * Validates input, initializes simulation data, and delegates to
 * appropriate handler (single philosopher or full simulation).
 * 
 * Expected arguments:
 * ./philo 
 * <num_philos> <time_to_die> <time_to_eat> <time_to_sleep> [must_eat_count]
 */
int	main(int argc, char **argv)
{
	t_data	*data;

	if (argc != 5 && argc != 6)
		return (ft_putstr(RED "Error: Wrong number of args\n" RS), 1);
	if (!validate_input(argc, argv))
		return (ft_putstr(RED "Error: Invalid arguments\n" RS), 1);
	data = init_data(argc, argv);
	if (!data)
		return (ft_putstr(RED "Error: Invalid args | init failed\n" RS), 1);
	if (data->num_philos == 1)
	{
		ft_putstr("0 1 has taken a fork\n");
		precise_sleep(data->time_to_die);
		ft_print_status(&data->philos[0], "died", RED);
		return (clean_up(data), 0);
	}
	if (!create_philosophers(data))
	{
		clean_up(data);
		return (ft_putstr(RED "Error: Failed to create threads\n" RS), 1);
	}
	monitor_philosophers(data);
	join_philosophers(data);
	return (clean_up(data), 0);
}
