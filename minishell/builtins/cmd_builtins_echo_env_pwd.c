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

void	builtin_echo(t_cmd *cmd, int *status, t_minishell *shell)
{
	int	newline;
	int	i;

	newline = 1;
	i = 1;
	*status = 0;
	if (!cmd->args[1])
	{
		printf("\n");
		return ;
	}
	while (cmd->args[i] && cmd->args[i][0] == '-'
		&& is_only_that_char(cmd->args[i] + 1, 'n'))
	{
		newline = 0;
		i++;
	}
	while (cmd->args[i])
	{
		parse_input(cmd->args[i], shell);
		if (cmd->args[i + 1])
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
}

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
	char	cwd[4096];

	getcwd(cwd, 4097);
	printf("%s\n", cwd);
	*status = 0;
}
