/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abendrih <abendrih@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 19:00:00 by abendrih          #+#    #+#             */
/*   Updated: 2025/11/21 02:28:07 by abendrih         ###   ########.fr       */
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
		if (key->type == TOKEN_WORD || key->type == TOKEN_WORD_DOUBLE_QUOTED
			|| key->type == TOKEN_WORD_SINGLE_QUOTED)
		{
			i++;
		}
		key = key->next;
	}
	return (i);
}

// char	**tokens_to_array(t_token **lst)
// {
// 	int		i;
// 	char	**args;
// 	t_token	*key;

// 	key = *lst;
// 	i = 0;
// 	args = malloc(sizeof(char *) * (count_tokens(lst) + 1));
// 	if (!args)
// 		return (NULL);
// 	while (key)
// 	{
// 		if (key->type != TOKEN_WORD && key->type != TOKEN_WORD_DOUBLE_QUOTED
// 			&& key->type != TOKEN_WORD_SINGLE_QUOTED)
// 		{
// 			args[i] = NULL;
// 			return (args);
// 		}
// 		args[i] = ft_strdup(key->value);
// 		i++;
// 		key = key->next;
// 	}
// 	args[i] = NULL;
// 	return (args);
// }

char	**tokens_to_array(t_token **lst)
{
	int		i;
	char	**args;
	t_token	*key;

	key = *lst;
	i = 0;
	args = malloc(sizeof(char *) * (count_tokens(lst) + 1));
	if (!args)
		return (NULL);
	while (key)
	{
		// 1. Si c'est un PIPE, on stop
		if (key->type == TOKEN_PIPE)
			break ;
		// 2. Si c'est une REDIRECTION, on saute 2 tokens
		if (key->type == TOKEN_REDIRECT_IN || key->type == TOKEN_REDIRECT_OUT
			|| key->type == TOKEN_APPEND || key->type == TOKEN_HEREDOC)
		{
			key = key->next; // Saute la redirection
			if (key)
				key = key->next; // Saute le fichier
			continue ;
		}
		// 3. Sinon c'est un WORD, on copie
		args[i] = ft_strdup(key->value);
		i++;
		key = key->next;
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
	char	*valid_cmd;
	int		i;

	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	dirs = ft_split(get_path_from_env(envp), ':');
	if (!dirs)
		return (NULL);
	i = 0;
	while (dirs[i])
	{
		tmp = ft_strjoin(dirs[i], "/");
		valid_cmd = ft_strjoin(tmp, cmd);
		if (access(valid_cmd, X_OK) == 0)
			return (ft_free_2(&dirs[i + 1]), free(dirs), valid_cmd);
		free(valid_cmd);
		i++;
	}
	return (free(dirs), NULL);
}
