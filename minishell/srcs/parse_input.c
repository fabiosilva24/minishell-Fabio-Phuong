#include "../include/minishell.h"

int handle_redirection(char **arg)
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
		while (**arg == ' ')
			(*arg)++;
		filename = *arg;
		if (*filename == '\0')
			return (-1);
		redirect_input(filename);
		return (1);
	}
		return (0);
}

void	parse_input(char *input)
{
	char *arg;
	int handled;
	
	handled = 0;
	arg = input;

	while (*arg)
	{
		if (*arg == '\"')
		{
			double_quotes(arg);
			handled = 1;
			break;
		}
		else if (*arg == '\'')
		{
			single_quotes(arg);
			handled = 1;
			break;
		}
		else if (handle_redirection(&arg))
		{
			*arg = '\0';
			handled = 1;
			continue;
		}
		arg++;
	}
}
