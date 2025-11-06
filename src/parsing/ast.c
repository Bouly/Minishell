/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abendrih <abendrih@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 02:30:00 by abendrih          #+#    #+#             */
/*   Updated: 2025/11/07 00:15:04 by abendrih         ###   ########.fr       */
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

static t_ast	*create_node(t_node_type type, char **args)
{
	t_ast	*key;

	key = malloc(sizeof(t_ast));
	if (!key)
		return (NULL);
	key->type = type;
	key->args = args;
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
	t_ast	*node;
	t_token	*left_tokens;

	// int		i;
	// 1. Chercher s'il y a un pipe
	pipe = find_pipe(tokens);
	if (pipe)
	{
		// 2. Créer un nœud PIPE
		node = create_node(NODE_PIPE, NULL);
		// 3. Parser la partie AVANT le pipe (left)
		left_tokens = befor_pipe(&tokens);
		node->left = parse(left_tokens);
		token_free(&left_tokens); // FREE la copie
		// 4. Parser la partie APRÈS le pipe (right)
		node->right = parse(pipe->next);
	}
	else
	{
		node = create_node(NODE_COMMAND, tokens_to_array(&tokens));
		// printf("DEBUG: Commande créée avec args: ");
		// i = 0;
		// while (node->args && node->args[i])
		// {
		// 	printf("[%s] ", node->args[i]);
		// 	i++;
		// }
		// printf("\n");
	}
	return (node);
}
