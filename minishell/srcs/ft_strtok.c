/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtok.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+        
	+:+     */
/*   By: fsilva-p <fsilva-p@student.42.fr>          +#+  +:+      
	+#+        */
/*                                                +#+#+#+#+#+  
	+#+           */
/*   Created: 2025/03/07 15:25:04 by fsilva-p          #+#    #+#             */
/*   Updated: 2025/03/07 15:25:04 by fsilva-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


char	*ft_strtok(char *str, const char *delimeter)
{
	char *token_start;
	static char *next_token = NULL;

	if (str)
		next_token = str;

	if (!next_token)
		return (NULL);

	while (*next_token == *delimeter)
	{
		next_token++;
	}
	if (*next_token == '\0')
		return (NULL);
	token_start = next_token;
	while (*next_token != *delimeter && *next_token != '\0')
		next_token++;
	if (*next_token == *delimeter)
	{
		*next_token = '\0';
		next_token++;
	}
	return (token_start);
}