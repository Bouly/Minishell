/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abendrih <abendrih@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 02:30:00 by abendrih          #+#    #+#             */
/*   Updated: 2025/11/12 17:03:21 by abendrih         ###   ########.fr       */
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

static t_ast	*create_three(t_node_type type, char **args)
{
	t_ast	*key;

	key = malloc(sizeof(t_ast));
	if (!key)
		return (NULL);
	key->type = type;
	key->args = args;
	key->infile = NULL;
	key->outfile = NULL;
	key->append = 0;
	key->heredoc_delim = NULL;
	key->right = NULL;
	key->left = NULL;
	return (key);
}

void	ast_free(t_ast **three)
{
	if (!three || !*three)
		return ;
	if ((*three)->type == NODE_PIPE)
	{
		ast_free(&(*three)->right);
		ast_free(&(*three)->left);
		free(*three);
	}
	else
	{
		ft_free((*three)->args);
		if ((*three)->infile)
			free((*three)->infile);
		if ((*three)->outfile)
			free((*three)->outfile);
		if ((*three)->heredoc_delim)
			free((*three)->heredoc_delim);
		free(*three);
	}
}

static t_token	*befor_pipe(t_token **tokens)
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
	t_ast	*three;
	t_token	*left_tokens;

	pipe = find_pipe(tokens);
	if (pipe)
	{
		three = create_three(NODE_PIPE, NULL);
		left_tokens = befor_pipe(&tokens);
		three->left = parse(left_tokens);
		token_free(&left_tokens);
		three->right = parse(pipe->next);
	}
	else
	{
		three = create_three(NODE_COMMAND, NULL);
		extract_redirections(&tokens, three);
		three->args = tokens_to_array(&tokens);
	}
	return (three);
}
