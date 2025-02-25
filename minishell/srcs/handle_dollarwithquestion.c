#include "../include/minishell.h"

/*void handle_dollar_questionmark(t_cmd *cmd, t_minishell *shell)
{
	int i;
	char *status_str;

	i = 0;

	while (cmd->args[i])
	{
		if (ft_strcmp(cmd->args[i], "$?") == 0)
		{
			status_str = ft_itoa(shell->exit_status);
			if (!status_str)
				free(cmd->args[i]);
			cmd->args[i] = status_str;
		}
		i++;
	}
}*/