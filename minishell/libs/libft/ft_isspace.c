/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isspace.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+        
	+:+     */
/*   By: fsilva-p <fsilva-p@student.42.fr>          +#+  +:+      
	+#+        */
/*                                                +#+#+#+#+#+  
	+#+           */
/*   Created: 2025/03/15 15:22:42 by fsilva-p          #+#    #+#             */
/*   Updated: 2025/03/15 15:22:42 by fsilva-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isspace(int argument)
{
	if (argument == ' ' || argument == '\t' || argument == '\n'
		|| argument == '\v' || argument == '\f' || argument == '\r')
		return (1);
	return (0);
}
