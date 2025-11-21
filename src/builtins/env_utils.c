/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abendrih <abendrih@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 01:00:00 by abendrih          #+#    #+#             */
/*   Updated: 2025/11/21 17:29:28 by abendrih         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static t_env	*env_new_node(const char *key, const char *value)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = ft_strdup((char *)key);
	node->value = ft_strdup((char *)value);
	if (!node->key || !node->value)
	{
		free(node->key);
		free(node->value);
		free(node);
		return (NULL);
	}
	node->next = NULL;
	return (node);
}

static int	env_parse_and_add(char *env_line, t_env **head)
{
	char	*eq;
	char	*key;
	t_env	*new_node;
	t_env	*current;

	eq = ft_strchr(env_line, '=');
	if (!eq)
		return (1);
	key = ft_substr(env_line, 0, eq - env_line);
	if (!key)
		return (0);
	new_node = env_new_node(key, eq + 1);
	free(key);
	if (!new_node)
		return (0);
	if (!*head)
		*head = new_node;
	else
	{
		current = *head;
		while (current->next)
			current = current->next;
		current->next = new_node;
	}
	return (1);
}

static void	init_default_env(t_env **env)
{
	char	cwd[PATH_MAX];
	char	*shlvl_str;
	int		shlvl;

	if (!env_get(*env, "PWD"))
	{
		if (getcwd(cwd, sizeof(cwd)))
			env_set(env, "PWD", cwd);
	}
	shlvl_str = env_get(*env, "SHLVL");
	if (shlvl_str)
	{
		shlvl = ft_atoi(shlvl_str) + 1;
		shlvl_str = ft_itoa(shlvl);
		env_set(env, "SHLVL", shlvl_str);
		free(shlvl_str);
	}
	else
		env_set(env, "SHLVL", "1");
	if (!env_get(*env, "_"))
		env_set(env, "_", "");
}

t_env	*env_init(char **envp)
{
	t_env	*head;
	int		i;

	head = NULL;
	if (envp && envp[0])
	{
		i = 0;
		while (envp[i])
		{
			env_parse_and_add(envp[i], &head);
			i++;
		}
	}
	init_default_env(&head);
	return (head);
}

void	env_free(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env;
		env = env->next;
		free(tmp->key);
		free(tmp->value);
		free(tmp);
	}
}
