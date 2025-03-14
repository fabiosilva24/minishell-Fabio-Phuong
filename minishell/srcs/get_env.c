/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-alm <joao-alm@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 15:23:42 by joao-alm          #+#    #+#             */
/*   Updated: 2025/03/14 15:40:28 by joao-alm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+
	+:+     */
/*   By: fsilva-p <fsilva-p@student.42.fr>          +#+  +:+
	+#+        */
/*                                                +#+#+#+#+#+
	+#+           */
/*   Created: 2025/01/30 17:56:57 by fsilva-p          #+#    #+#             */
/*   Updated: 2025/01/30 17:57:11 by fsilva-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static	char	**alloc_env(int count)
{
	char	**env;

	env = malloc(sizeof(char *) * (count + 1));
	if (!env)
		return (NULL);
	return (env);
}

static int	count_environment(void)
{
	int	count;

	count = 0;
	while (environ[count])
		count++;
	return (count);
}

char	**get_environment(void)
{
	char	**env;
	int		i;
	int		count;

	count = count_environment();
	env = alloc_env(count);
	i = 0;
	while (environ[i])
	{
		env[i] = ft_strdup(environ[i]);
		if (!env[i])
		{
			while (i > 0)
			{
				free(env[i - 1]);
				i--;
			}
			free(env);
			return (NULL);
		}
		i++;
	}
	env[i] = NULL;
	return (env);
}
