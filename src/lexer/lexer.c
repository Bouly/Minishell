/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abendrih <abendrih@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 04:18:54 by abendrih          #+#    #+#             */
/*   Updated: 2025/11/02 16:53:56 by abendrih         ###   ########.fr       */
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

static int	handle_quote(char *line, int *i, t_token **head)
{
	char	quote;
	char	*word;
	int		token_type;

	quote = line[*i];
	word = extract_quoted_word(line, i, quote);
	if (word == NULL)
	{
		ft_putstr_fd("syntax error: unclosed quote\n", 2);
		return (0);
	}
	if (quote == '"')
		token_type = TOKEN_WORD_DOUBLE_QUOTED;
	else
		token_type = TOKEN_WORD_SINGLE_QUOTED;
	token_addback(head, token_new(token_type, word));
	free(word);
	return (1);
}

static int	handle_word(char *line, int *i, t_token **head)
{
	int		start;
	char	*word;

	start = *i;
	while (line[*i] != '\0' && !is_space(line[*i]) && !is_operator(line[*i]))
		(*i)++;
	word = ft_substr(line, start, *i - start);
	if (word == NULL)
		return (0);
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
		else if (is_quote(line[i]))
			ok = handle_quote(line, &i, &head);
		else
			ok = handle_word(line, &i, &head);
		if (ok == 0)
			return (token_free(&head), NULL);
	}
	return (head);
}
