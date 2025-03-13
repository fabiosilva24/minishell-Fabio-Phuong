/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+
	+:+     */
/*   By: fsilva-p <fsilva-p@student.42.fr>          +#+  +:+
	+#+        */
/*                                                +#+#+#+#+#+
	+#+           */
/*   Created: 2025/03/12 20:18:40 by fsilva-p          #+#    #+#             */
/*   Updated: 2025/03/12 20:18:40 by fsilva-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	handle_parse_things(char **arg, int *handled, t_minishell *shell)
{
	int	i;
	int	len;

	i = 1;
	len = ft_strlen(*arg);
	if (**arg == '\"' || **arg == '\'')
	{
		if (**arg == '\"')
			double_quotes(*arg, shell);
		else
			single_quotes(*arg);
		*handled = 1;
	}
	else if (handle_redirection(arg))
		*handled = 1;
	else if (**arg == '$')
	{
		if (*(*arg + 1) == '$')
			printf("SHELL PID: %d", ft_getpid());
		else
			handle_dollarsign(*arg, &i, len, shell);
		*handled = 1;
	}
}

void	parse_input(char *input, t_minishell *shell)
{
	char	*arg;
	int		handled;
	char	*input_copy;

	handled = 0;
	input_copy = ft_strdup(input);
	if (!input_copy)
		return ;
	expand_exit_status(&input_copy, shell->environment, shell->exit_status);
	arg = input_copy;
	while (*arg && !handled)
	{
		handle_parse_things(&arg, &handled, shell);
		arg++;
	}
	if (!handled)
		ft_printf("%s", input_copy);
	free(input_copy);
}
