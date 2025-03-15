/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+        
	+:+     */
/*   By: fsilva-p <fsilva-p@student.42.fr>          +#+  +:+      
	+#+        */
/*                                                +#+#+#+#+#+  
	+#+           */
/*   Created: 2025/03/15 15:26:52 by fsilva-p          #+#    #+#             */
/*   Updated: 2025/03/15 15:26:52 by fsilva-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strndup(const char *s1, size_t n)
{
	char	*copy;
	size_t	len;

	len = ft_strlen(s1);
	if (len > n)
		len = n;
	copy = (char *)malloc(len + 1);
	if (!copy)
		return (NULL);
	ft_strncpy(copy, s1, len);
	copy[len] = '\0';
	return (copy);
}
