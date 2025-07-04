/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aybelaou <aybelaou@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 19:11:16 by aybelaou          #+#    #+#             */
/*   Updated: 2025/07/04 15:21:00 by aybelaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosopher.h"

long long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

bool	is_simulation_over(t_data *data)
{
	bool	result;

	pthread_mutex_lock(&data->end_mutex);
	result = data->simulation_end;
	pthread_mutex_unlock(&data->end_mutex);
	return (result);
}

void	precise_sleep(long long ms)
{
	long long	end_time;

	end_time = get_time() + ms;
	while (get_time() < end_time)
		usleep(50);
}

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
