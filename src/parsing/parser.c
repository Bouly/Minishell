/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abendrih <abendrih@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 02:30:00 by abendrih          #+#    #+#             */
/*   Updated: 2025/11/21 05:56:27 by abendrih         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

	pipe = find_pipe(tokens);
	if (pipe)
	{
		tree = create_ast(NODE_PIPE, NULL);
		left_tokens = before_pipe(&tokens);
		if (left_tokens->type == TOKEN_PIPE)
		{
			ft_putstr_fd("bash: syntax error near unexpected token `||'\n", 2);
			token_free(&left_tokens);
			free(tree);
			return (NULL);
		}
		tree->left = parse(left_tokens);
		token_free(&left_tokens);
		if (!pipe->next || pipe->next->type == TOKEN_PIPE)
		{
			ft_putstr_fd("bash: syntax error near unexpected token `||'\n", 2);
			ast_free(&tree);
			return (NULL);
		}
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
