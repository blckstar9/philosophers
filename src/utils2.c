/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aybelaou <aybelaou@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 19:45:04 by aybelaou          #+#    #+#             */
/*   Updated: 2025/07/05 14:34:06 by aybelaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file utils2.c
 * @brief String utilities and safe integer conversion
 * 
 * Provides safe string to integer conversion with overflow protection
 * and basic string length calculation.
 */

#include "../inc/philosopher.h"

/**
 * @brief Parse digits from string with overflow checking
 * @param str Pointer to string pointer (modified during parsing)
 * @param result Pointer to result accumulator
 * @param sign Sign multiplier (1 or -1)
 * @param err Pointer to error flag
 * @return 1 on success, 0 on overflow
 * 
 * Helper function for ft_atoi_safe. Accumulates digits while
 * checking for integer overflow/underflow.
 */
static int	ft_parse_digits(const char **str, long *result, int sign, bool *err)
{
	while (**str >= '0' && **str <= '9')
	{
		*result = *result * 10 + (**str - '0');
		if ((sign == 1 && *result > INT_MAX)
			|| (sign == -1 && (-*result) < INT_MIN))
		{
			*err = true;
			return (0);
		}
		(*str)++;
	}
	return (1);
}

/**
 * @brief Safe string to integer conversion with error detection
 * @param str String to convert
 * @param error Pointer to error flag (set to true on error)
 * @return Converted integer value, 0 on error
 * 
 * Converts string to integer with comprehensive error checking:
 * - Overflow/underflow detection
 * - Invalid character detection
 * - Proper sign handling
 * - Whitespace skipping
 */
int	ft_atoi_safe(const char *str, bool *error)
{
	long	result;
	int		sign;

	result = 0;
	sign = 1;
	*error = false;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-')
		sign = -1;
	if (*str == '-' || *str == '+')
		str++;
	if (!ft_parse_digits(&str, &result, sign, error))
		return (0);
	if (*str != '\0')
		*error = true;
	return ((int)(result * sign));
}

/**
 * @brief Calculate string length
 * @param str Input string
 * @return Number of characters in string (excluding null terminator)
 * 
 * Standard string length calculation. Used by printing functions
 * to determine write() buffer sizes.
 */
int	ft_strlen(const char *str)
{
	int	len;

	len = 0;
	while (str[len] != '\0')
		len++;
	return (len);
}
