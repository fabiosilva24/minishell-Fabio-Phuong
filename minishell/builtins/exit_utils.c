#include "../include/minishell.h"

static char *handle_exit_status(char *var_name, int last_exit_status)
{
	//free(var_name);
	if (!ft_strncmp("?", var_name, 1))
		return (ft_itoa(last_exit_status));
	return (ft_strdup(""));
}

static char *find_env_variable(char **envp, char *var_name)
{
	int i;
	int eq_pos;

	i = -1;
	while (envp[++i])
	{
		eq_pos = ft_len_eq(envp[i]);
		if (eq_pos == (int)ft_strlen(envp[i])
			|| !ft_strncmp(var_name, envp[i], max(eq_pos, ft_strlen(var_name))))
		{
			free(var_name);
			if (eq_pos == (int)ft_strlen(envp[i]))
				return (ft_strdup(""));
			return (ft_strdup(envp[i] + eq_pos + 1));
		}
	}
	return (NULL);
}

char	*get_exit_variable_value(char **envp, char *line,
	int len, int last_exit_status)
{
	char	*var_name;
	char	*env_value;

	var_name = malloc(sizeof(char) * len);
	if (!var_name)
		return (0);
	ft_strlcpy(var_name, line, len);

	env_value = find_env_variable(envp, var_name);
	if (env_value)
		return (env_value);
	return (handle_exit_status(var_name, last_exit_status));
}
