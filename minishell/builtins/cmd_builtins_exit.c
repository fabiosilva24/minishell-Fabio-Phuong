
#include "../include/minishell.h"

static int	valid_exit_code(const char *arg)
{
	int		i;
	int		len;
	char	*max_long;

	if (!arg || !arg[0] || ((arg[0] == '-' || arg[0] == '+') && !arg[1]))
		return (0);
	i = (arg[0] == '-' || arg[0] == '+');
	len = 0;
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
			return (0);
		len++;
		i++;
	}
	max_long = "9223372036854775807";
	if (len > 19)
		return (0);
	if (len == 19)
	{
		if (arg[0] == '-')
			max_long = "9223372036854775808";
		if (ft_strncmp(
				arg + (arg[0] == '-' || arg[0] == '+'), 
				max_long,
				19) > 0)
			return (0);
	}
	return (1);
}

static void	handle_exit_with_arg(t_cmd *cmd, t_minishell *shell, int should_exit)
{
	long	exit_code;

	if (!valid_exit_code(cmd->args[1]))
	{
		printf("exit\n");
		printf("minishell: exit: %s: numeric argument required\n", cmd->args[1]);
		shell->exit_status = 2;
		return ;
	}
	exit_code = ft_atol(cmd->args[1]);
	shell->exit_status = (unsigned char)exit_code;
	if (should_exit)
	{
		printf("exit\n");
		exit(shell->exit_status);
	}
}

void	builtin_exit(t_cmd *cmd, t_minishell *shell, int should_exit)
{
	if (!cmd)
	{
		shell->status = 1;
		return ;
	}
	if (cmd->args[1] && cmd->args[2])
	{
		printf("exit\n");
		printf("minishell: exit: too many arguments\n");
		shell->exit_status = 1;
		return ;
	}
	if (cmd->args[1])
		handle_exit_with_arg(cmd, shell, should_exit);
	else
	{
		shell->exit_status = 0;
		if (should_exit)
		{
			printf("exit\n");
			exit(0);
		}
	}
}
