#include "../include/minishell.h"

char	*create_pwd_var(void)
{
	char	cwd[MAX_PATH_LEN];
	char	*pwd;

	pwd = ft_strjoin("PWD=", getcwd(cwd, 4096));
	return (pwd);
}

char	**update_env_vars(char **envp, char *pwd_var, char *oldpwd_var)
{
	envp = replace_env_var(envp, pwd_var, "PWD=");
	if (is_env_var_present("OLDPWD=", envp))
		envp = replace_env_var(envp, oldpwd_var, "OLDPWD=");
	else
		envp = add_env_var(envp, oldpwd_var, 1);
	return (envp);
}

char	**update_pwd_oldpwd(char **envp, char *old_path, int *status)
{
	char	*pwd;
	char	*new_old_path;

	(void)status;
	new_old_path = create_oldpwd_var(old_path);
	if (!new_old_path)
		return (envp);
	pwd = create_pwd_var();
	if (!pwd)
	{
		free(new_old_path);
		free(old_path);
		return (envp);
	}
	envp = update_env_vars(envp, pwd, new_old_path);
	free(new_old_path);
	free(pwd);
	free(old_path);
	return (envp);
}
