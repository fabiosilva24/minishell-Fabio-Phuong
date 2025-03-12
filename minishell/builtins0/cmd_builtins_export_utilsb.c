#include "../include/minishell.h"

static void	swap_strings(char **str1, char **str2)
{
	char	*tmp;

	tmp = ft_strdup(*str1);
	free(*str1);
	*str1 = ft_strdup(*str2);
	free(*str2);
	*str2 = ft_strdup(tmp);
	free(tmp);
}

static void	sort_one_pass(char **mass, int j, int i)
{
	int	k;

	k = -1;
	while (++k < (j - i - 1))
	{
		if (ft_strncmp(mass[k], mass[k + 1],
				ft_sym_export(mass[k])) > 0)
			swap_strings(&mass[k], &mass[k + 1]);
	}
}

char	**sort_env_vars(char **mass)
{
	int	i;
	int	j;

	i = -1;
	j = size_mass(mass);
	while (++i < (j - 1))
		sort_one_pass(mass, j, i);
	mass[i + 1] = NULL;
	return (mass);
}
