/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aybelaou <aybelaou@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 18:46:18 by aybelaou          #+#    #+#             */
/*   Updated: 2025/06/05 23:04:22 by aybelaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosopher.h"

// Clean up allocated resources
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
	pthread_mutex_destroy(&data->meal_mutex);  // Add this line
	if (data->philos)
		free(data->philos);
	free(data);
}

// Create philosopher threads
bool	create_philosophers(t_data *data)
{
    int	i;

    data->start_time = get_time();
    i = 0;
    while (i < data->num_philos)
    {
        data->philos[i].last_meal_time = data->start_time;
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

// Join philosopher threads
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

int	main(int argc, char **argv)
{
    t_data	*data;

    if (argc != 5 && argc != 6)
    {
        printf(RED "Error: Wrong number of arguments\n" RS);
        printf("Usage: %s number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]\n", argv[0]);
        return (1);
    }
    data = init_data(argc, argv);
    if (!data)
        return (printf(RED "Error: Invalid arguments or initialization failed\n" RS), 1);
    if (data->num_philos == 1)
    {
        printf("0 1 has taken a fork\n");
        precise_sleep(data->time_to_die);
        printf("%lld 1 died\n", (long long)data->time_to_die);
        clean_up(data);
        return (0);
    }
	if (!create_philosophers(data))
	{
		clean_up(data);
		return (printf(RED "Error: Failed to create threads\n" RS), 1);
	}
	monitor_philosophers(data);
	join_philosophers(data);
	clean_up(data);
	return (0);
}
