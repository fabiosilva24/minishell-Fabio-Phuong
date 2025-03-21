/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsilva-p <fsilva-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 15:28:39 by fsilva-p          #+#    #+#             */
/*   Updated: 2025/03/15 15:28:39 by fsilva-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

long	ft_atol(const char *str)
{
	long	i;
	int		sign;

	i = 0;
	while ((*str >= 9 && *str <= 13) || *str == ' ')
		str++;
	sign = 1;
	if (*str == '-' || *str == '+')
		sign = 44 - *str++;
	while (ft_isdigit(*str))
		i = (i * 10) + (*str++ - '0');
	return (i * sign);
}

/*int main() 
{
    char str[] = "  12345";
    int num = ft_atoi(str);
    write(1, &num, sizeof(num));
    return 0;
}*/