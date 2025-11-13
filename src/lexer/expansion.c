/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abendrih <abendrih@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 00:00:00 by abendrih          #+#    #+#             */
/*   Updated: 2025/11/13 00:00:00 by abendrih         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	is_valid_var_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

static char	*extract_var_name(char *str, int *i)
{
	int		start;
	char	*var_name;

	(*i)++;
	if (str[*i] == '?')
	{
		(*i)++;
		return (ft_strdup("?"));
	}
	start = *i;
	while (str[*i] && is_valid_var_char(str[*i]))
		(*i)++;
	if (*i == start)
		return (ft_strdup(""));
	var_name = ft_substr(str, start, *i - start);
	return (var_name);
}

static char	*get_var_value(t_shell *shell, char *var_name)
{
	char	*value;
	char	*exit_str;

	if (!var_name || var_name[0] == '\0')
		return (ft_strdup("$"));
	if (var_name[0] == '?' && var_name[1] == '\0')
	{
		exit_str = ft_itoa(shell->last_exit_status);
		return (exit_str);
	}
	value = get_env_value(shell, var_name);
	if (value)
		return (ft_strdup(value));
	return (ft_strdup(""));
}

char	*expand_variables(char *str, t_shell *shell)
{
	char	*result;
	char	*var_name;
	char	*var_value;
	int		i;
	int		start;

	result = ft_strdup("");
	i = 0;
	while (str[i])
	{
		start = i;
		while (str[i] && str[i] != '$')
			i++;
		if (i > start)
			result = ft_strjoin(result, ft_substr(str, start, i - start));
		if (str[i] == '$')
		{
			var_name = extract_var_name(str, &i);
			var_value = get_var_value(shell, var_name);
			result = ft_strjoin(result, var_value);
			free(var_name);
			free(var_value);
		}
	}
	return (result);
}
