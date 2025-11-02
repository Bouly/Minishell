/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abendrih <abendrih@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 09:29:21 by abendrih          #+#    #+#             */
/*   Updated: 2025/11/02 16:10:36 by abendrih         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*get_path_from_env(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (strncmp(envp[i], "PATH=", 5) == 0)
			return (envp[i] + 5);
		i++;
	}
	return (NULL);
}

char	*find_command(char *cmd, char **envp)
{
	char	**dirs;
	char	*tmp;
	char	*path;
	char	*valid_cmd;
	int		i;

	i = 0;
	path = get_path_from_env(envp);
	dirs = ft_split(path, ':');
	while (dirs[i])
	{
		tmp = ft_strjoin(dirs[i], "/");
		valid_cmd = ft_strjoin(tmp, cmd);
		if (access(valid_cmd, X_OK) == 0)
		{
			ft_free_2(&dirs[i + 1]);
			free(dirs);
			return (valid_cmd);
		}
		free(valid_cmd);
		i++;
	}
	free(dirs);
	return (NULL);
}
