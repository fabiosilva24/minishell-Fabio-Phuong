/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phoang <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 15:53:31 by phoang            #+#    #+#             */
/*   Updated: 2025/03/11 16:06:36 by phoang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	errmsg(char *s1, char *s2, char *s3, int code, int *status)
{
	ft_putstr_fd(s1, 2);
	ft_putstr_fd(s2, 2);
	ft_putstr_fd(s3, 2);
	ft_putstr_fd("\n", 2);
	if (code < 0)
		*status = -code;
	else
		exit(code);
}

int	size_mass(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
		i++;
	return (i);
}

char	**new_envp(char **envp)
{
	char	**new_mass;
	int		i;

	new_mass = malloc(sizeof(char *) * (size_mass(envp) + 1));
	if (!new_mass)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		new_mass[i] = ft_strdup(envp[i]);
		i++;
	}
	new_mass[i] = NULL;
	return (new_mass);
}

int	ft_sym_export(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	return (i);
}

void	ft_free(char **mass)
{
	int	sizemass;

	sizemass = 0;
	while (mass[sizemass])
	{
		free(mass[sizemass]);
		sizemass++;
	}
	free(mass);
}
