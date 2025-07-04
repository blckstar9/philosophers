/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_status.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aybelaou <aybelaou@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 15:22:48 by aybelaou          #+#    #+#             */
/*   Updated: 2025/07/04 15:48:16 by aybelaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosopher.h"

void	ft_putstr(const char *str)
{
	if (!str)
		return ;
	write(1, str, ft_strlen(str));
}

static int	ft_numlen(long long n)
{
	int	len;

	len = 0;
	if (n <= 0)
		len = 1;
	while (n != 0)
	{
		n /= 10;
		len++;
	}
	return (len);
}

static void	ft_lltoa(long long n, char *str)
{
	int			len;
	int			i;
	long long	nb;

	len = ft_numlen(n);
	i = len - 1;
	nb = n;
	if (nb == 0)
		str[0] = '0';
	if (nb < 0)
	{
		str[0] = '-';
		nb = -nb;
	}
	while (nb > 0)
	{
		str[i] = (nb % 10) + '0';
		nb /= 10;
		i--;
	}
	str[len] = '\0';
}

static int	copy_string(char *buffer, int pos, char *str)
{
	while (*str)
		buffer[pos++] = *str++;
	return (pos);
}

void	ft_print_status(t_philo *philo, char *status, char *color)
{
	char		buffer[256];
	char		time_str[32];
	char		id_str[16];
	long long	current_time;
	int			pos;

	pthread_mutex_lock(&philo->data->print_mutex);
	if (!is_simulation_over(philo->data))
	{
		current_time = get_time() - philo->data->start_time;
		ft_lltoa(current_time, time_str);
		ft_lltoa(philo->id, id_str);
		pos = 0;
		if (color)
			pos = copy_string(buffer, pos, color);
		pos = copy_string(buffer, pos, time_str);
		buffer[pos++] = ' ';
		pos = copy_string(buffer, pos, id_str);
		buffer[pos++] = ' ';
		pos = copy_string(buffer, pos, status);
		pos = copy_string(buffer, pos, "\033[0m\n");
		buffer[pos] = '\0';
		write(1, buffer, pos);
	}
	pthread_mutex_unlock(&philo->data->print_mutex);
}
