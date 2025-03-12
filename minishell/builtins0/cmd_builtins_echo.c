#include "../include/minishell.h"

static int	is_only_that_char(char *str, char c)
{
	while (*str)
	{
		if (*str != c)
			return (0);
		str++;
	}
	return (1);
}

static int	handle_n_option(char **args, int *i)
{
	int	newline;

	newline = 1;
	while (args[*i] && args[*i][0] == '-'
		&& is_only_that_char(args[*i] + 1, 'n'))
	{
		newline = 0;
		(*i)++;
	}
	return (newline);
}

static void	print_arguments(char **args, int i, t_minishell *shell)
{
	while (args[i])
	{
		parse_input(args[i], shell);
		if (args[i + 1])
			printf(" ");
		i++;
	}
}

void	builtin_echo(t_cmd *cmd, int *status, t_minishell *shell)
{
	int	newline;
	int	i;

	i = 1;
	*status = 0;
	if (!cmd->args[1])
	{
		printf("\n");
		return ;
	}
	newline = handle_n_option(cmd->args, &i);
	print_arguments(cmd->args, i, shell);
	if (newline)
		printf("\n");
}
