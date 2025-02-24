#include "../include/minishell.h"

void	free_tab(char **tab)
{
	int	i;

	if (!tab)
		return ;
	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		tab[i] = NULL;
		i++;
	}
	free(tab);
}

int	errmsg(char *s1, char *s2, char *s3, int code)
{
	ft_putstr_fd(s1, 2);
	ft_putstr_fd(s2, 2);
	ft_putstr_fd(s3, 2);
	ft_putstr_fd("\n", 2);
	return (-code);
}

int	size_mass(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
		i++;
	return (i);
}

char	**new_envp(char **envp)
{
	char	**new_mass;
	int		i;

	new_mass = malloc(sizeof(char *) * (size_mass(envp) + 1));
	if (!new_mass)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		new_mass[i] = ft_strdup(envp[i]);
		if (!new_mass[i])
		{
			free_tab(new_mass);
			return (NULL);
		}
		i++;
	}
	new_mass[i] = NULL;
	return (new_mass);
}

int ft_sym_export(char *str)
{
    int i;

    i = 0;
    while (str[i] && str[i] != '=')
        i++;
    return (i);
}
