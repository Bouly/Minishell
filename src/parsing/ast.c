/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abendrih <abendrih@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 19:00:00 by abendrih          #+#    #+#             */
/*   Updated: 2025/11/12 20:04:30 by abendrih         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
** Crée un nouveau nœud d'arbre syntaxique
** Initialise tous les champs à NULL/0
** Retourne: nœud AST alloué
*/
t_ast	*create_ast(t_node_type type, char **args)
{
	t_ast	*node;

	node = malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	node->type = type;
	node->args = args;
	node->infile = NULL;
	node->outfile = NULL;
	node->append = 0;
	node->heredocs = NULL;
	node->right = NULL;
	node->left = NULL;
	return (node);
}

/*
** Libère récursivement tout l'arbre syntaxique
** Libère args, fichiers, heredocs et nœuds enfants
** Paramètres: tree - pointeur sur la racine à libérer
*/
void	ast_free(t_ast **tree)
{
	if (!tree || !*tree)
		return ;
	if ((*tree)->type == NODE_PIPE)
	{
		ast_free(&(*tree)->right);
		ast_free(&(*tree)->left);
		free(*tree);
	}
	else
	{
		ft_free((*tree)->args);
		if ((*tree)->infile)
			free((*tree)->infile);
		if ((*tree)->outfile)
			free((*tree)->outfile);
		if ((*tree)->heredocs)
			heredoc_free(&(*tree)->heredocs);
		free(*tree);
	}
}
