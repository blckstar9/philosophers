/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aybelaou <aybelaou@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 19:11:05 by aybelaou          #+#    #+#             */
/*   Updated: 2025/06/05 23:04:01 by aybelaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosopher.h"

// Parse command line arguments
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

// Initialize mutexes
bool	init_mutexes(t_data *data)
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
            // Clean up previously initialized mutexes
            while (--i >= 0)
                pthread_mutex_destroy(&data->forks[i]);
            free(data->forks);
            return (false);
        }
        i++;
    }
    if (pthread_mutex_init(&data->print_mutex, NULL) != 0)
    {
        // Clean up all fork mutexes
        i = 0;
        while (i < data->num_philos)
            pthread_mutex_destroy(&data->forks[i++]);
        free(data->forks);
        return (false);
    }
    if (pthread_mutex_init(&data->end_mutex, NULL) != 0)
    {
        // Clean up all previously initialized mutexes
        i = 0;
        while (i < data->num_philos)
            pthread_mutex_destroy(&data->forks[i++]);
        pthread_mutex_destroy(&data->print_mutex);
        free(data->forks);
        return (false);
    }
    if (pthread_mutex_init(&data->meal_mutex, NULL) != 0)
    {
        // Clean up all previously initialized mutexes
        i = 0;
        while (i < data->num_philos)
            pthread_mutex_destroy(&data->forks[i++]);
        pthread_mutex_destroy(&data->print_mutex);
        pthread_mutex_destroy(&data->end_mutex);
        free(data->forks);
        return (false);
    }
    return (true);
}

// Initialize philosophers
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
        data->philos[i].right_fork = &data->forks[(i + 1) % data->num_philos];
        i++;
    }
    return (true);
}

// Initialize data structure
t_data	*init_data(int argc, char **argv)
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	if (!data)
		return (NULL);
	memset(data, 0, sizeof(t_data));
	data->simulation_end = false;
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
