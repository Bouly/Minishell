/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_util.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abendrih <abendrih@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 15:31:08 by abendrih          #+#    #+#             */
/*   Updated: 2025/11/02 16:07:08 by abendrih         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_space(char c)
{
	return (c == ' ' || c == '\t');
}

int	is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

int	is_quote(char c)
{
	return (c == '\'' || c == '"');
}

int	skip_spaces(const char *line, int i)
{
	while (line[i] != '\0' && is_space(line[i]))
		i++;
	return (i);
}

char	*extract_quoted_word(char *line, int *i, char quote_char)
{
	char	*word;
	int		start;

	start = *i + 1;
	(*i)++;
	while (line[*i] && line[*i] != quote_char)
		(*i)++;
	if (line[*i] == '\0')
	{
		return (NULL);
	}
	word = ft_substr(line, start, *i - start);
	(*i)++;
	return (word);
}
