/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abendrih <abendrih@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 02:30:00 by abendrih          #+#    #+#             */
/*   Updated: 2025/11/12 20:04:30 by abendrih         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	is_redirect_token(t_token_type type)
{
	return (type == TOKEN_REDIRECT_IN || type == TOKEN_REDIRECT_OUT
		|| type == TOKEN_APPEND || type == TOKEN_HEREDOC);
}

static int	validate_syntax(t_token *tokens)
{
	t_token	*current;

	current = tokens;
	if (!current)
		return (1);
	if (current->type == TOKEN_PIPE)
		return (ft_putstr_fd("syntax error near unexpected token `|'\n", 2), 0);
	while (current)
	{
		if (current->type == TOKEN_PIPE)
		{
			if (!current->next || current->next->type == TOKEN_PIPE)
				return (ft_putstr_fd("syntax error near unexpected token `|'\n", 2), 0);
		}
		if (is_redirect_token(current->type))
		{
			if (!current->next || current->next->type == TOKEN_PIPE
				|| is_redirect_token(current->next->type))
				return (ft_putstr_fd("syntax error near unexpected token `newline'\n", 2), 0);
		}
		current = current->next;
	}
	return (1);
}

t_token	*find_pipe(t_token *tokens)
{
	t_token	*key;

	key = tokens;
	while (key && key->type != TOKEN_PIPE)
	{
		key = key->next;
	}
	return (key);
}

static t_token	*before_pipe(t_token **tokens)
{
	t_token	*key;
	t_token	*tmp;

	key = *tokens;
	tmp = token_new(key->type, key->value);
	key = key->next;
	while (key && key->type != TOKEN_PIPE)
	{
		token_addback(&tmp, token_new(key->type, key->value));
		key = key->next;
	}
	return (tmp);
}

t_ast	*parse(t_token *tokens)
{
	t_token	*pipe;
	t_ast	*tree;
	t_token	*left_tokens;

	if (!validate_syntax(tokens))
		return (NULL);
	pipe = find_pipe(tokens);
	if (pipe)
	{
		tree = create_ast(NODE_PIPE, NULL);
		left_tokens = before_pipe(&tokens);
		tree->left = parse(left_tokens);
		token_free(&left_tokens);
		tree->right = parse(pipe->next);
	}
	else
	{
		tree = create_ast(NODE_COMMAND, NULL);
		extract_redirections(&tokens, tree);
		tree->args = tokens_to_array(&tokens);
	}
	return (tree);
}
