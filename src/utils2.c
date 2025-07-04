/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aybelaou <aybelaou@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 19:45:04 by aybelaou          #+#    #+#             */
/*   Updated: 2025/07/04 15:49:20 by aybelaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosopher.h"

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

int	ft_strlen(const char *str)
{
	int	len;

	len = 0;
	while (str[len] != '\0')
		len++;
	return (len);
}
