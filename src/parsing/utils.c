/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abendrih <abendrih@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 19:00:00 by abendrih          #+#    #+#             */
/*   Updated: 2025/11/12 20:04:30 by abendrih         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	count_tokens(t_token **lst)
{
	int		i;
	t_token	*key;

	i = 0;
	key = *lst;
	while (key)
	{
		key = key->next;
		i++;
	}
	return (i);
}

char	**tokens_to_array(t_token **lst)
{
	int		i;
	char	**args;

	i = 0;
	args = malloc(sizeof(char *) * (count_tokens(lst) + 1));
	if (!args)
		return (NULL);
	while (*lst)
	{
		if ((*lst)->type != TOKEN_WORD
			&& (*lst)->type != TOKEN_WORD_DOUBLE_QUOTED
			&& (*lst)->type != TOKEN_WORD_SINGLE_QUOTED)
		{
			args[i] = NULL;
			if ((*lst)->next)
				*lst = (*lst)->next;
			return (args);
		}
		args[i] = ft_strdup((*lst)->value);
		i++;
		*lst = (*lst)->next;
	}
	args[i] = NULL;
	return (args);
}

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
