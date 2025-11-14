/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abendrih <abendrih@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 01:00:00 by abendrih          #+#    #+#             */
/*   Updated: 2025/11/14 01:00:00 by abendrih         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

#ifndef PATH_MAX
# define PATH_MAX 4096
#endif

static char	*get_target_dir(char **args, t_env *env)
{
	char	*path;

	if (!args[1] || ft_strcmp(args[1], "~") == 0)
	{
		path = env_get(env, "HOME");
		if (!path)
		{
			ft_putstr_fd("cd: HOME not set\n", 2);
			return (NULL);
		}
		return (path);
	}
	if (ft_strcmp(args[1], "-") == 0)
	{
		path = env_get(env, "OLDPWD");
		if (!path)
		{
			ft_putstr_fd("cd: OLDPWD not set\n", 2);
			return (NULL);
		}
		ft_putstr_fd(path, 1);
		ft_putstr_fd("\n", 1);
		return (path);
	}
	return (args[1]);
}

static void	update_pwd_vars(t_env **env)
{
	char	cwd[PATH_MAX];
	char	*old_pwd;

	old_pwd = env_get(*env, "PWD");
	if (old_pwd)
		env_set(env, "OLDPWD", old_pwd);
	if (getcwd(cwd, sizeof(cwd)))
		env_set(env, "PWD", cwd);
}

int	builtin_cd(char **args, t_env **env)
{
	char	*path;

	path = get_target_dir(args, *env);
	if (!path)
		return (1);
	if (chdir(path) != 0)
	{
		ft_putstr_fd("cd: ", 2);
		perror(path);
		return (1);
	}
	update_pwd_vars(env);
	return (0);
}
