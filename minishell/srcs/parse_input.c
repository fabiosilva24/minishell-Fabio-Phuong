#include "../include/minishell.h"


int	handle_redirection(char **arg)
{
	char *filename;

	if (**arg == '>')
	{
		(*arg)++;
		if (**arg == '>')
			(*arg)++;

		while (**arg == ' ')
			(*arg)++;

		filename = *arg;
		if (*filename == '\0')
			return (-1);
		if (*(*arg - 1) == '>')
			redirect_output_append(filename);
		else
			redirect_output(filename);
		return (1);
	}
	else if (**arg == '<')
	{
		(*arg)++;
		if (**arg == '<')
		{
			(*arg)++;
			while (**arg == ' ')
				(*arg)++;
			filename = *arg;
			while (**arg && **arg != ' ' && **arg != '\n')
				(*arg)++;
			**arg = '\0';
			if (*filename == '\0')
				return (-1);
			return (handle_heredoc(filename));
		}
		while (**arg == ' ')
			(*arg)++;
		filename = *arg;
		while (**arg && **arg != ' ' && **arg != '\n')
			(*arg)++;
		**arg = '\0';
		redirect_input(filename);
		return (1);
	}
	return (0);
}

void	parse_input(char *input, t_minishell *shell)
{
	char *arg;
	int handled;
	int i;
	int len;
	char *input_copy;

	i = 1;
	handled = 0;
	input_copy = ft_strdup(input);
	if (!input_copy)
		return;

	expand_exit_status(&input_copy, shell->environment, shell->exit_status);
	arg = input_copy;
	len = ft_strlen(arg);
	
	while (*arg)
	{
		if (*arg == '\"')
		{
			double_quotes(arg, shell);
			handled = 1;
			break ;
		}
		else if (*arg == '\'')
		{
			single_quotes(arg);
			handled = 1;
			break ;
		}
		else if (handle_redirection(&arg))
		{
			handled = 1;
			break ;
		}
		else if (*arg == '$')
		{
			handle_dollarsign(arg, &i, len, shell);
			handled = 1;
			break ;
		}
		arg++;
	}
	if (!handled)
		printf("%s", input_copy);
	free(input_copy);
}
