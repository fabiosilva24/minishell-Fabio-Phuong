#include "../include/minishell.h"

/*
char	*get_env_var_value(char **envp, char *line, int len);
int		sym_export(t_minishell *shell, char *var);
int		setenv(char *env_var, char *value);
*/

static char	*find_env_var(char **envp, char *var_name)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (!ft_strncmp(var_name, envp[i],
			max(len_eq(envp[i]), ft_strlen(var_name))))
			return (ft_strdup(envp[i] + len_eq(envp[i]) + 1));
		i++;
	}
	return (NULL);
}

char	*get_env_var_value(char **envp, char *line, int len)
{
	char	*var_name;
	char	*value;

	var_name = malloc(sizeof(char) * (len + 1));
	if (!var_name)
		return (NULL);
	ft_strlcpy(var_name, line, len + 1);
	value = find_env_var(envp, var_name);
	free(var_name);

	if (value)
		return (value);
	if (!ft_strncmp("?", var_name, 1))
		return (ft_itoa(g_line.status));
	return (NULL);
}

int	sym_export(t_minishell *shell, char *var)
{
	char	*env_var;
	char	*value;
	int		result;

	if (!var || !shell)
		return (-1);
	env_var = strdup(var);
	if (!env_var)
		return (-1);
	value = strdup("");
	if (!value)
	{
		free(env_var);
		return (-1);
	}
	result = ft_setenv(env_var, value);
	free(env_var);
	free(value);
	return (result);
}

static char	*create_env_entry(const char *key, const char *value)
{
	char	*tmp;
	char	*entry;

	tmp = ft_strjoin(key, "=");
	if (!tmp)
		return (NULL);
	entry = ft_strjoin(tmp, value);
	free(tmp);
	return (entry);
}

static int	update_existing_env(char **envp, const char *key, const char *new_entry)
{
	int	i;
	int	len;

	len = ft_strlen(key);
	i = 0;
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], key, len) && envp[i][len] == '=')
		{
			free(envp[i]);
			envp[i] = new_entry;
			return (1);
		}
		i++;
	}
	return (0);
}

static int	add_new_env_entry(t_minishell *shell, char *new_entry)
{
	char	**new_env;
	int		i;

	i = 0;
	while (shell->envp[i])
		i++;
	new_env = malloc(sizeof(char *) * (i + 2));
	if (!new_env)
		return (free(new_entry), 1);
	i = 0;
	while (shell->envp[i])
	{
		new_env[i] = shell->envp[i];
		i++;
	}
	new_env[i] = new_entry;
	new_env[i + 1] = NULL;
	free(shell->envp);
	shell->envp = new_env;
	return (0);
}

int	ft_setenv(t_minishell *shell, char *key, char *value)
{
	char	*new_entry;

	if (!shell || !key || !value)
		return (1);
	new_entry = create_env_entry(key, value);
	if (!new_entry)
		return (1);
	if (update_existing_env(shell->envp, key, new_entry))
		return (0);
	return (add_new_env_entry(shell, new_entry));
}
