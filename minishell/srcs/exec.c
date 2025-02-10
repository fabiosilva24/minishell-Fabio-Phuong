#include "../minishell.h"

/*
int		exec_process(t_cmd *cmd, char **envp);
int		exec_cmd(t_cmd *cmd, char **envp);
void	wait_for_all_processes(void);
int		handle_builtin(t_cmd *cmd, t_minishell *shell);
void	handle_exit_status(int status, t_minishell *shell);
*/

int	exec_process(t_cmd *cmd, char **envp)
{
	char	*cmd_path;
	int		status;

	status = check_cmd_validity(cmd);
	if (status != 0)
		return (status);
	cmd_path = get_exec_path(cmd, envp);
	if (!cmd_path)
		return (127);
	if (execve(cmd_path, cmd->args, envp) == -1)
	{
		status = handle_execve_error(cmd_path, cmd->args[0]);
		free(cmd_path);
		return (status);
	}
	free(cmd_path);
	return (0);
}

int	exec_cmd(t_cmd *cmd, char **envp)
{
	pid_t	pid;
	int		status;

	if (!cmd || !envp)
		return (-1);
	if (is_builtin(cmd))
		return (handle_builtin(cmd));
	pid = fork();
	if (pid == -1)
	{
		write(2, "minishell: fork failed\n", 23);
		return (-1);
	}
	if (pid == 0)
	{
		status = exec_process(cmd, envp);
		exit(status);
	}
	waitpid(pid, &status, 0);
	return (status >> 8);
}

void	wait_for_all_processes(void)
{
	int	status;
	pid_t	pid;

	while (1)
	{
		pid = wait(&status);
		if (pid == -1)
			break ;
	}
}

int	handle_builtin(t_cmd *cmd, t_minishell *shell)
{
	if (!cmd || !cmd->args[0])
		return (0);
	if (ft_strcmp(cmd->args[0], "cd") == 0)
		return (builtin_cd(cmd, shell));
	if (ft_strcmp(cmd->args[0], "echo") == 0)
		return (builtin_echo(cmd));
	if (ft_strcmp(cmd->args[0], "exit") == 0)
		return (builtin_exit(cmd, shell));
	if (ft_strcmp(cmd->args[0], "env") == 0)
		return (builtin_env(shell));
	if (ft_strcmp(cmd->args[0], "pwd") == 0)
		return (builtin_pwd());
	if (ft_strcmp(cmd->args[0], "export") == 0)
		return (builtin_export(cmd, shell));
	if (ft_strcmp(cmd->args[0], "unset") == 0)
		return (builtin_unset(cmd, shell));
	return (0);
}

void	handle_exit_status(int status, t_minishell *shell)
{
	if (status >= 0 && status <= 255)
		shell->exit_status = status;
	else
		shell->exit_status = 128 + (status % 256);
}
