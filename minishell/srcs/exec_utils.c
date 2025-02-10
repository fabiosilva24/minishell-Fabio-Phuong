#include "../minishell.h"

/*
void	free_tab(char **tab);
int		check_cmd_validity(t_cmd *cmd);
int		handle_execve_error(char *cmd_path, char *cmd);
void	print_cmd_error(const char *cmd, const char *message);
*/
void	free_tab(char **tab)
{
    int	i;

    if (!tab)
        return ;
    i = 0;
    while (tab[i])
    {
        free(tab[i]);
        i++;
    }
    free(tab);
    tab = NULL;
}

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

if (access(cmd_path, X_OK) == -1)
   {
       print_cmd_error(cmd, ": Permission denied\n");
       ret = 126;
   }
   else
   {
       print_cmd_error(cmd, ": execution failed\n");
       ret = 127;
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
		ft_putstr_fd(" ", 2);
    ft_putstr_fd(message, 2);
    ft_putstr_fd("\n", 2);
}
