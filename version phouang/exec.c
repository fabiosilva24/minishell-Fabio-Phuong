#include "../minishell.h"

int	is_builtin(t_cmd *cmd)
{
	if (!cmd || !cmd->args[0])
		return (0);
	if (ft_strcmp(cmd->args[0], "cd") == 0 ||
		ft_strcmp(cmd->args[0], "echo") == 0 ||
		ft_strcmp(cmd->args[0], "exit") == 0 ||
		ft_strcmp(cmd->args[0], "env") == 0 ||
		ft_strcmp(cmd->args[0], "pwd") == 0 ||
		ft_strcmp(cmd->args[0], "export") == 0 ||
		ft_strcmp(cmd->args[0], "unset") == 0)
		return (1);
	return (0);
}

int exec_process(t_cmd *cmd, char **envp)
{
	char	*cmd_path;
	int		status;
	
	if (!cmd || !envp)
		return (1);
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
		exit(status);
	}
	free(cmd_path);
	return (0);
}

int exec_cmd(t_cmd *cmd, t_minishell *shell)
{
   pid_t   pid;
   int     status;

   if (!cmd || !shell || !shell->envp)
       return (-1);
   if (is_builtin(cmd))
       return (handle_builtin(cmd, shell));
   pid = fork();
   if (pid == -1)
   {
       print_cmd_error("fork", ": failed\n");
       return (-1);
   }
   if (pid == 0)
   {
       status = exec_process(cmd, shell->envp);
       exit(status);
   }
   waitpid(pid, &status, 0);
   handle_exit_status(status, shell);
   return (shell->exit_status);
}

void wait_for_all_processes(void)
{
	int		status;

	while (waitpid(-1, &status, 0) > 0)
		;
}
