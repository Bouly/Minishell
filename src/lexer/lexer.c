/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abendrih <abendrih@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 04:18:54 by abendrih          #+#    #+#             */
/*   Updated: 2025/11/10 22:04:30 by abendrih         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	handle_pipe(int *i, t_token **head)
{
	token_addback(head, token_new(TOKEN_PIPE, "|"));
	*i += 1;
	return (1);
}

static int	handle_redirect(const char *line, int *i, t_token **head)
{
	char	c;

	c = line[*i];
	if (c == '>')
	{
		if (line[*i + 1] == '>')
		{
			token_addback(head, token_new(TOKEN_APPEND, ">>"));
			*i += 2;
			return (1);
		}
		token_addback(head, token_new(TOKEN_REDIRECT_OUT, ">"));
		(*i)++;
		return (1);
	}
	if (line[*i + 1] == '<')
	{
		token_addback(head, token_new(TOKEN_HEREDOC, "<<"));
		*i += 2;
		return (1);
	}
	token_addback(head, token_new(TOKEN_REDIRECT_IN, "<"));
	(*i)++;
	return (1);
}

static char	*accumulate_adjacent_parts(char *line, int *i)
{
	char	*result;
	char	*part;
	int		start;

	result = ft_strdup("");
	while (line[*i] && !is_space(line[*i]) && !is_operator(line[*i]))
	{
		if (is_quote(line[*i]))
		{
			part = extract_quoted_word(line, i, line[*i]);
			if (!part)
				return (free(result), NULL);
			result = ft_strjoin(result, part);
			free(part);
		}
		else
		{
			start = *i;
			while (line[*i] && !is_space(line[*i]) && !is_operator(line[*i])
				&& !is_quote(line[*i]))
				(*i)++;
			part = ft_substr(line, start, *i - start);
			result = ft_strjoin(result, part);
			free(part);
		}
	}
	return (result);
}

static int	handle_word(char *line, int *i, t_token **head)
{
	char	*word;

	word = accumulate_adjacent_parts(line, i);
	if (word == NULL)
		return (0);
	if (word[0] == '\0')
	{
		free(word);
		return (1);
	}
	token_addback(head, token_new(TOKEN_WORD, word));
	free(word);
	return (1);
}

t_token	*lexer(char *line)
{
	t_token	*head;
	int		i;
	int		ok;

	head = NULL;
	i = 0;
	while (line[i] != '\0')
	{
		i = skip_spaces(line, i);
		if (line[i] == '\0')
			break ;
		ok = 1;
		if (line[i] == '|')
			ok = handle_pipe(&i, &head);
		else if (line[i] == '>' || line[i] == '<')
			ok = handle_redirect(line, &i, &head);
		else // ← Enlève le "else if (is_quote...)"
			ok = handle_word(line, &i, &head);
		if (ok == 0)
			return (token_free(&head), NULL);
	}
	return (head);
}
