#include "../minishell.h"

int	check_cmd_validity(t_cmd *cmd)
{
	if (!cmd || !cmd->args || !cmd->args[0])
	{
		print_cmd_error("(null)", ": command not found\n");
		return (127);
	}
	return (0);
}

int	handle_execve_error(char *cmd_path, char *cmd)
{
	int ret;

	ret = 127;
	if (cmd_path && access(cmd_path, F_OK) == -1)
	{
		print_cmd_error(cmd, ": No such file or directory\n");
	}
	else if (cmd_path && access(cmd_path, X_OK) == -1)
	{
		print_cmd_error(cmd, ": Permission denied\n");
		ret = 126;
	}
	else
	{
		print_cmd_error(cmd, ": execution failed\n");
	}
	return (ret);
}

void	print_cmd_error(const char *cmd, const char *message)
{
		if (!cmd)
				cmd = "(null)";
		if (!message)
			message = "";
    ft_putstr_fd("minishell: ", 2);
    ft_putstr_fd(cmd, 2);
    ft_putstr_fd(message, 2);
	if (message[0] && message[ft_strlen(message) - 1] != '\n')
		ft_putstr_fd("\n", 2);
}
