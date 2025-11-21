/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expension.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abendrih <abendrih@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 18:25:27 by abendrih          #+#    #+#             */
/*   Updated: 2025/11/21 18:59:59 by abendrih         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*get_var_name(char *str, int *i)
{
	int	start;

	start = *i;
	(*i)++;
	if (str[*i] == '?')
	{
		(*i)++;
		return (ft_strndup("?", 1));
	}
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	return (ft_strndup(&str[start + 1], *i - start - 1));
}

static char	*get_var_value(char *var_name, t_env *env, int exit_status)
{
	char	*value;

	if (ft_strcmp(var_name, "?") == 0)
		return (ft_itoa(exit_status));
	value = env_get(env, var_name);
	if (!value)
		return (ft_strdup(""));
	return (ft_strdup(value));
}

char	*expand_variables(char *str, t_env *env, int exit_status)
{
	char	*result;
	char	*var_name;
	char	*var_value;
	int		i;
	char	*tmp_char;

	result = ft_strdup("");
	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && (ft_isalnum(str[i + 1]) || str[i + 1] == '_'
				|| str[i + 1] == '?'))
		{
			var_name = get_var_name(str, &i);
			var_value = get_var_value(var_name, env, exit_status);
			result = ft_strjoin(result, var_value);
			free(var_name);
			free(var_value);
		}
		else
		{
			tmp_char = ft_strndup(&str[i], 1);
			result = ft_strjoin(result, tmp_char);
			free(tmp_char);
			i++;
		}
	}
	return (result);
}
