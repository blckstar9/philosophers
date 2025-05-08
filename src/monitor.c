/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aybelaou <aybelaou@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 19:11:09 by aybelaou          #+#    #+#             */
/*   Updated: 2025/05/08 19:19:01 by aybelaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosopher.h"

// Check if all philosophers have eaten required meals
bool	all_ate_enough(t_data *data)
{
    int	i;
    
    if (data->must_eat_count == -1)
        return (false);
    
    i = 0;
    while (i < data->num_philos)
    {
        if (data->philos[i].meals_eaten < data->must_eat_count)
            return (false);
        i++;
    }
    return (true);
}

// Set simulation end flag
void	set_simulation_end(t_data *data, bool value)
{
    pthread_mutex_lock(&data->end_mutex);
    data->simulation_end = value;
    pthread_mutex_unlock(&data->end_mutex);
}

// Monitor philosophers for death or completion
void	monitor_philosophers(t_data *data)
{
    int		i;
    long long	current_time;
    
    while (1)
    {
        i = 0;
        while (i < data->num_philos)
        {
            current_time = get_time();
            if ((current_time - data->philos[i].last_meal_time) > data->time_to_die)
            {
                print_status(&data->philos[i], "died", RED);
                set_simulation_end(data, true);
                return;
            }
            i++;
        }
        
        if (all_ate_enough(data))
        {
            set_simulation_end(data, true);
            return;
        }
        
        usleep(1000);  // Small sleep to reduce CPU load
    }
}