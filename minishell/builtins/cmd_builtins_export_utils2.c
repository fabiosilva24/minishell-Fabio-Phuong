/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_builtins_export_utils2.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phoang <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 20:07:15 by phoang            #+#    #+#             */
/*   Updated: 2025/03/12 20:07:24 by phoang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static char	*extract_single_name(char *env_var)
{
	int		j;
	char	*var_name;

	var_name = malloc(sizeof(char) * (ft_sym_export(env_var) + 1));
	if (!var_name)
		return (NULL);
	if (!ft_strchr(env_var, '='))
		return (ft_strdup(env_var));
	j = -1;
	while (env_var[++j] != '=')
		var_name[j] = env_var[j];
	var_name[j] = '\0';
	return (var_name);
}

char	**extract_var_names(char **env_copy_tmp)
{
	int		i;
	char	**var_names;

	if (!env_copy_tmp)
		return (NULL);
	var_names = malloc(sizeof(char *) * (size_mass(env_copy_tmp) + 1));
	if (!var_names)
		return (NULL);
	i = -1;
	while (env_copy_tmp[++i])
	{
		var_names[i] = extract_single_name(env_copy_tmp[i]);
		if (!var_names[i])
			return (NULL);
	}
	var_names[i] = NULL;
	return (var_names);
}

void	print_sorted_env(char **envp, int *status)
{
	int		i;
	char	**env_copy_tmp;
	char	**var_names;
	char	**var_values;

	i = 0;
	env_copy_tmp = new_envp(envp);
	var_names = extract_var_names(env_copy_tmp);
	var_values = extract_var_values(env_copy_tmp);
	sort_env_vars(var_names);
	while (var_names && var_names[i])
	{
		printf("declare -x %s=\"%s\"\n", var_names[i], var_values[i]);
		i++;
	}
	ft_free(var_names);
	ft_free(var_values);
	ft_free(env_copy_tmp);
	*status = 0;
}

char	**change_envp(char *new_value, char **args, int i, char **envp)
{
	if (!envp)
		return (NULL);
	if (new_value && ft_strrchr(args[i], '='))
		envp = replace_env_var(envp, args[i], new_value);
	else if (!new_value)
		envp = add_env_var(envp, args[i], 0);
	return (envp);
}
