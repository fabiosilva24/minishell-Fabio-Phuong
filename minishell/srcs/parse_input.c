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

	arg = input;

	//printf("input : %s\n", input);
	while (*arg)
	{
		if (*arg == '\"')
		{
			double_quotes(arg);
			break;
		}
		else if (*arg == '\'')
		{
			single_quotes(arg);
			break;
		}
		else if (handle_redirection(&arg))
		{
			*arg = '\0';
			continue;
		}
		else
		{
			printf("%c", *arg);
		}
		arg++;
	}
}
