/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aybelaou <aybelaou@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 18:46:18 by aybelaou          #+#    #+#             */
/*   Updated: 2025/05/08 19:19:46 by aybelaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosopher.h"

#include "../inc/philosopher.h"

// Function prototypes for linking


// Clean up allocated resources
void	clean_up(t_data *data)
{
    int	i;
    
    if (!data)
        return;
    
    // Destroy mutexes
    if (data->forks)
    {
        for (i = 0; i < data->num_philos; i++)
            pthread_mutex_destroy(&data->forks[i]);
        free(data->forks);
    }
    
    pthread_mutex_destroy(&data->print_mutex);
    pthread_mutex_destroy(&data->end_mutex);
    
    // Free philosophers array
    if (data->philos)
        free(data->philos);
    
    // Free data structure
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
        return (printf(RED "Error: Wrong number of arguments\n" RS), 1);
    
    data = init_data(argc, argv);
    if (!data)
        return (printf(RED "Error: Invalid arguments or initialization failed\n" RS), 1);
    
    if (!create_philosophers(data))
    {
        clean_up(data);
        return (printf(RED "Error: Failed to create threads\n" RS), 1);
    }
    
    // Special case for 1 philosopher
    if (data->num_philos == 1)
    {
        printf("%lld %d has taken a fork\n", 0LL, 1);
        precise_sleep(data->time_to_die);
        printf("%lld %d died\n", (long long)data->time_to_die, 1);
        clean_up(data);
        return (0);
    }
    
    // Monitor philosophers
    monitor_philosophers(data);
    
    // Wait for threads to finish
    join_philosophers(data);
    
    // Clean up resources
    clean_up(data);
    return (0);
}