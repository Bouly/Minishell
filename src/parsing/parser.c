/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abendrih <abendrih@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 02:30:00 by abendrih          #+#    #+#             */
/*   Updated: 2025/11/25 21:14:27 by abendrih         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
** Trouve le premier pipe dans la liste de tokens
** Utilisé pour diviser la commande en sous-arbres gauche/droite
** Retourne: token pipe trouvé ou NULL
*/
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

/*
** Copie tous les tokens avant le premier pipe
** Crée une nouvelle liste pour construire le sous-arbre gauche
** Retourne: nouvelle liste de tokens
*/
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

/*
** Vérifie les erreurs de syntaxe liées aux pipes
** Détecte les pipes en début/fin ou multiples consécutifs
** Retourne: 1 si OK, 0 si erreur de syntaxe
*/
static int	handle_pipe_error(t_token *left_tokens, t_token *pipe, t_ast *tree)
{
	if (left_tokens->type == TOKEN_PIPE)
	{
		ft_putstr_fd("bash: syntax error near unexpected token `||'\n", 2);
		token_free(&left_tokens);
		free(tree);
		return (0);
	}
	if (!pipe->next || pipe->next->type == TOKEN_PIPE)
	{
		ft_putstr_fd("bash: syntax error near unexpected token `||'\n", 2);
		token_free(&left_tokens);
		ast_free(&tree);
		return (0);
	}
	return (1);
}

/*
** Vérifie si une redirection est suivie d'un token valide
** Retourne: 1 si OK, 0 si erreur de syntaxe
*/
static int	validate_redirect_syntax(t_token *tokens)
{
	t_token *(current) = tokens;
	while (current)
	{
		if (current->type == TOKEN_REDIRECT_IN
			|| current->type == TOKEN_REDIRECT_OUT
			|| current->type == TOKEN_APPEND || current->type == TOKEN_HEREDOC)
		{
			if (!current->next || current->next->type == TOKEN_PIPE
				|| current->next->type == TOKEN_REDIRECT_IN
				|| current->next->type == TOKEN_REDIRECT_OUT
				|| current->next->type == TOKEN_APPEND
				|| current->next->type == TOKEN_HEREDOC)
			{
				ft_putstr_fd("bash: syntax error near unexpected token `", 2);
				if (!current->next)
					ft_putstr_fd("newline", 2);
				else
					ft_putstr_fd(current->next->value, 2);
				return (ft_putstr_fd("'\n", 2), 0);
			}
		}
		current = current->next;
	}
	return (1);
}

/*
** Parse récursivement les tokens en arbre syntaxique (AST)
** Gère les pipes et construit l'arbre gauche/droite
** Retourne: nœud AST ou NULL si erreur
*/
t_ast	*parse(t_token *tokens)
{
	t_ast	*tree;
	t_token	*pipe;
	t_token	*left_tokens;

	if (!validate_redirect_syntax(tokens))
		return (NULL);
	pipe = find_pipe(tokens);
	if (pipe)
	{
		tree = create_ast(NODE_PIPE, NULL);
		left_tokens = before_pipe(&tokens);
		if (!handle_pipe_error(left_tokens, pipe, tree))
			return (NULL);
		free((tree->left = parse(left_tokens), token_free(&left_tokens), NULL));
		tree->right = parse(pipe->next);
		if (!tree->left || !tree->right)
			return (ast_free(&tree), NULL);
	}
	else
	{
		tree = create_ast(NODE_COMMAND, NULL);
		extract_redirections(&tokens, tree);
		tree->args = tokens_to_array(&tokens);
	}
	return (tree);
}
