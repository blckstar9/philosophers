/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aybelaou <aybelaou@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 19:11:09 by aybelaou          #+#    #+#             */
/*   Updated: 2025/05/30 20:09:23 by aybelaou         ###   ########.fr       */
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
    int			i;
    long long	current_time;
    long long	time_since_meal_start;

    while (1)
    {
        i = 0;
        while (i < data->num_philos)
        {
            pthread_mutex_lock(&data->meal_mutex);
            current_time = get_time();
            time_since_meal_start = current_time - data->philos[i].last_meal_time;
            // Problem: This checks from meal START, not meal END
            if (time_since_meal_start > data->time_to_die)  // Change >= to >
            {
                print_status(&data->philos[i], "died", RED);
                pthread_mutex_unlock(&data->meal_mutex);
                set_simulation_end(data, true);
                return ;
            }
            pthread_mutex_unlock(&data->meal_mutex);
            i++;
        }
        if (all_ate_enough(data))
        {
            set_simulation_end(data, true);
            return ;
        }
        usleep(1000);  // Increase back to 1000 for less CPU usage
    }
}
