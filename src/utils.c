/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aybelaou <aybelaou@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 19:11:16 by aybelaou          #+#    #+#             */
/*   Updated: 2025/06/05 23:04:29 by aybelaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosopher.h"

long long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	print_status(t_philo *philo, char *status, char *color)
{
	long long	current_time;

	pthread_mutex_lock(&philo->data->print_mutex);
	if (!is_simulation_over(philo->data))
	{
		current_time = get_time() - philo->data->start_time;
		printf("%s%lld %d %s%s\n", color, current_time, philo->id, status, RS);
	}
	pthread_mutex_unlock(&philo->data->print_mutex);
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

int	ft_atoi_safe(const char *str, bool *error)
{
	long	result;
	int		sign;

	result = 0;
	sign = 1;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-')
		sign = -1;
	if (*str == '-' || *str == '+')
		str++;
	while (*str >= '0' && *str <= '9')
	{
		result = result * 10 + (*str - '0');
		if ((sign == 1 && result > INT_MAX)
			|| (sign == -1 && (-result) < INT_MIN))
		{
			*error = true;
			return (0);
		}
		str++;
	}
	if (*str != '\0')
		*error = true;
	return ((int)(result * sign));
}

bool	validate_input(int argc, char **argv)
{
    int		i;
    bool	error;

    i = 1;
    while (i < argc)
    {
        if (ft_atoi_safe(argv[i], &error) <= 0 || error)
        {
            printf(RED "Error: Argument %d must be a positive integer\n" RS, i);
            return (false);
        }
        i++;
    }
    if (ft_atoi_safe(argv[1], &error) > 200)
    {
        printf(RED "Warning: Large number of philosophers may cause performance issues\n" RS);
    }
    return (true);
}
