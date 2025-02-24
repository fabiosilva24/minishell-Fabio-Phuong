#include "../include/minishell.h"

char	*get_variable_name(char *env_entry)
{
	int		len;
	char	*name;

	len = ft_sym_export(env_entry);
	name = malloc(sizeof(char) * (len + 1));
	if (!name)
		return (NULL);
	ft_strncpy(name, env_entry, len);
	name[len] = '\0';
	return (name);
}

void	free_variable_names(char **variable_names, int i)
{
	while (--i >= 0)
		free(variable_names[i]);
	free(variable_names);
}

int	print_env_entry(char *before, char *after, char *entry)
{
	write(1, "declare -x ", 11);
	write(1, before, ft_strlen(before));
	if (ft_strchr(entry, '='))
	{
		write(1, "=\"", 2);
		write(1, after, ft_strlen(after));
		write(1, "\"", 1);
	}
	write(1, "\n", 1);
	return (0);
}

int	is_valid_identifier(char *str)
{
	int i;

	if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
		return (0);
	i = 1;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

void	update_envp(char *new_value, char *arg, char ***envp)
{
	char **tmp;

	if (!envp || !*envp || !arg)
		return;
	if (new_value && ft_strrchr(arg, '='))
		tmp = replace_env_var(*envp, arg, new_value);
	else
		tmp = add_to_envp(*envp, arg, 0);
	if (!tmp)
	{
		errmsg("minishell: export: memory allocation error", NULL, NULL, -1);
		return;
	}
	free_tab(*envp);
	*envp = tmp;
}
