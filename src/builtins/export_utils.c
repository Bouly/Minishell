/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abendrih <abendrih@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 01:00:00 by abendrih          #+#    #+#             */
/*   Updated: 2025/11/16 17:35:43 by abendrih         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	env_set(t_env **env, const char *key, const char *value)
{
	t_env	*curr;
	t_env	*new;

	curr = *env;
	while (curr && ft_strcmp(curr->key, key) != 0)
		curr = curr->next;
	if (curr)
		return (free(curr->value), curr->value = ft_strdup((char *)value),
			curr->value != NULL);
	new = malloc(sizeof(t_env));
	if (!new)
		return (0);
	new->key = ft_strdup((char *)key);
	new->value = ft_strdup((char *)value);
	if (!new->key || !new->value)
		return (free(new->key), free(new->value), free(new), 0);
	new->next = NULL;
	if (!*env)
		return (*env = new, 1);
	curr = *env;
	while (curr->next)
		curr = curr->next;
	return (curr->next = new, 1);
}

int	env_unset(t_env **env, const char *key)
{
	t_env	*current;
	t_env	*prev;

	current = *env;
	prev = NULL;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				*env = current->next;
			free(current->key);
			free(current->value);
			free(current);
			return (1);
		}
		prev = current;
		current = current->next;
	}
	return (0);
}

static char	*create_env_string(t_env *env)
{
	char	*tmp;
	char	*result;

	tmp = ft_strdup(env->key);
	if (!tmp)
		return (NULL);
	tmp = ft_strjoin(tmp, "=");
	if (!tmp)
		return (NULL);
	result = ft_strjoin(tmp, env->value);
	return (result);
}

char	**env_to_array(t_env *env)
{
	char	**arr;
	t_env	*tmp;
	int		i;

	i = 0;
	tmp = env;
	while (tmp && ++i)
		tmp = tmp->next;
	if (!((arr = malloc(sizeof(char *) * (i + 1)))))
		return (NULL);
	i = 0;
	while (env)
	{
		if (!(arr[i] = create_env_string(env)))
		{
			while (i-- > 0)
				free(arr[i]);
			return (free(arr), NULL);
		}
		env = env->next;
		i++;
	}
	return (arr[i] = NULL, arr);
}
