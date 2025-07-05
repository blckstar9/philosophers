/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_status.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aybelaou <aybelaou@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 15:22:48 by aybelaou          #+#    #+#             */
/*   Updated: 2025/07/05 14:35:27 by aybelaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file print_status.c
 * @brief Optimized printing functions for thread-safe output
 * 
 * Implements high-performance printing using single write() calls
 * for precise timing and clean output formatting.
 */

#include "../inc/philosopher.h"

/**
 * @brief Print string to stdout
 * @param str String to print
 * 
 * Simple wrapper around write() with null pointer protection.
 * Used for error messages and simple string output.
 */
void	ft_putstr(const char *str)
{
	if (!str)
		return ;
	write(1, str, ft_strlen(str));
}

/**
 * @brief Calculate number of digits in a long long
 * @param n Number to analyze
 * @return Number of digits (including sign for negative numbers)
 * 
 * Helper function for ft_lltoa to determine buffer size needed.
 */
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

/**
 * @brief Convert long long to string
 * @param n Number to convert
 * @param str Buffer to store result (must be large enough)
 * 
 * Converts integer to string representation. Handles negative
 * numbers and zero correctly. Used for timestamp and ID formatting.
 */
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

/**
 * @brief Copy string to buffer starting at position
 * @param buffer Destination buffer
 * @param pos Starting position in buffer
 * @param str Source string to copy
 * @return New position after copied string
 * 
 * Helper function for building formatted output strings.
 * Returns updated position for chaining multiple copies.
 */
static int	copy_string(char *buffer, int pos, char *str)
{
	while (*str)
		buffer[pos++] = *str++;
	return (pos);
}

/**
 * @brief Thread-safe status printing with precise timing
 * @param philo Pointer to philosopher structure
 * @param status Status message to print
 * @param color Color code for message (can be NULL)
 * 
 * Optimized printing function using single write() call:
 * - Builds complete formatted string in buffer
 * - Includes timestamp, philosopher ID, status, and color codes
 * - Thread-safe using print_mutex
 * - Skips printing if simulation has ended
 * 
 * Output format: "[timestamp] [id] [status]"
 */
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
