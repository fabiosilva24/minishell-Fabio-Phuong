/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_builtins_export_utilsb.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phoang <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 20:07:34 by phoang            #+#    #+#             */
/*   Updated: 2025/03/12 20:07:38 by phoang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	swap_strings(char **str1, char **str2)
{
	char	*tmp;

	tmp = ft_strdup(*str1);
	free(*str1);
	*str1 = ft_strdup(*str2);
	free(*str2);
	*str2 = ft_strdup(tmp);
	free(tmp);
}

static void	sort_one_pass(char **mass, int j, int i)
{
	int	k;

	k = -1;
	while (++k < (j - i - 1))
	{
		if (ft_strncmp(mass[k], mass[k + 1],
				ft_sym_export(mass[k])) > 0)
			swap_strings(&mass[k], &mass[k + 1]);
	}
}

char	**sort_env_vars(char **mass)
{
	int	i;
	int	j;

	i = -1;
	j = size_mass(mass);
	while (++i < (j - 1))
		sort_one_pass(mass, j, i);
	mass[i + 1] = NULL;
	return (mass);
}


char	**add_env_var(char **envp, char *str, int free_old)
{
	char	**new_env;
	int		j;

	j = 0;
	new_env = malloc(sizeof(char *) * (size_mass(envp) + 2));
	if (!new_env)
		return (NULL);
	if (!copy_existing_vars(new_env, envp, &j))
		return (NULL);
	if (!add_new_var(new_env, str, j))
		return (NULL);
	new_env[j + 1] = NULL;
	if (free_old && envp)
		ft_free(envp);
	return (new_env);
}

char	**extract_var_values(char **tmpmass)
{
	int		i;
	int		j;
	int		k;
	char	**after;

	after = malloc(sizeof(char *) * (size_mass(tmpmass) + 1));
	if (!after)
		return (NULL);
	i = -1;
	while (tmpmass[++i])
	{
		k = 0;
		after[i] = malloc(sizeof(char) * (ft_strlen(tmpmass[i])
					- ft_sym_export(tmpmass[i]) + 1));
		if (!after[i])
			return (NULL);
		j = ft_sym_export(tmpmass[i]);
		while (tmpmass[i][++j] != '\0')
			after[i][k++] = tmpmass[i][j];
		after[i][k] = '\0';
	}
	after[i] = NULL;
	return (after);
}
