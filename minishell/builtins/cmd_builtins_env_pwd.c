/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_builtins_echo_env_pwd.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phoang <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 14:47:33 by phoang            #+#    #+#             */
/*   Updated: 2025/03/11 14:58:49 by phoang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	builtin_env(char **envp, int *status)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (!ft_strchr(envp[i], '='))
			i++;
		else
		{
			printf("%s\n", envp[i]);
			i++;
		}
	}
	*status = 0;
}

void	builtin_pwd(int *status)
{
	char	cwd[MAX_PATH_LEN + 1];

	getcwd(cwd, sizeof(cwd));
	printf("%s\n", cwd);
	*status = 0;
}
