#include "../include/minishell.h"

int handle_redirection(char **arg)
{
	char *filename;

	if (**arg == '>')
	{
		if (*(*arg + 1) == '>')
		{
			(*arg)++;
			filename = *arg + 1;
			while (*filename == ' ')
				filename++;
			redirect_output_append(filename);
		}
		else
		{
			filename = *arg + 1;
			while (*filename == ' ')
				filename++;
			redirect_output(filename);
		}
		return 1;
	}
	else if
	{
		filename = *arg + 1;
		while (*filename == ' ')
			filename++;
		redirect_input(filename);
		return (1);
	}
		return (0);
}

void	parse_input(char *input)
{
	char *arg = input;
	int handled = 0;

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
			handled = 1;
			break;
		}
		arg++;
	}
	if (!handled)
	{
		printf("%s", input);
	}
}
