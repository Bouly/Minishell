/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abendrih <abendrih@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 00:00:00 by abendrih          #+#    #+#             */
/*   Updated: 2025/11/23 17:35:30 by abendrih         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
** Libère tous les heredocs d'une liste
** Ferme les file descriptors et libère les délimiteurs
** Paramètres: lst - liste à libérer
*/
void	heredoc_free(t_heredoc **lst)
{
	t_heredoc	*current;
	t_heredoc	*next;

	if (!lst || !*lst)
		return ;
	current = *lst;
	while (current)
	{
		next = current->next;
		if (current->delim)
			free(current->delim);
		if (current->fd != -1)
			close(current->fd);
		free(current);
		current = next;
	}
	*lst = NULL;
}

/*
** Ferme tous les file descriptors des heredocs
** Met les fd à -1 après fermeture
** Paramètres: heredocs - liste des heredocs
*/
void	heredoc_close_all_fds(t_heredoc *heredocs)
{
	t_heredoc	*current;

	current = heredocs;
	while (current)
	{
		if (current->fd != -1)
		{
			close(current->fd);
			current->fd = -1;
		}
		current = current->next;
	}
}

/*
** Ferme récursivement tous les heredocs de l'AST
** Parcourt l'arbre et ferme les fds de chaque commande
** Paramètres: ast - racine de l'arbre
*/
void	close_all_ast_heredocs(t_ast *ast)
{
	if (!ast)
		return ;
	if (ast->type == NODE_PIPE)
	{
		close_all_ast_heredocs(ast->left);
		close_all_ast_heredocs(ast->right);
	}
	else if (ast->type == NODE_COMMAND)
	{
		if (ast->heredocs)
			heredoc_close_all_fds(ast->heredocs);
	}
}

/*
** Traite récursivement tous les heredocs de l'AST
** Parcourt l'arbre et traite les heredocs de chaque commande
** Retourne: 0 si succès, -1 si erreur ou interruption
*/
int	process_all_heredocs(t_ast *ast, t_shell *shell)
{
	if (!ast)
		return (0);
	if (ast->type == NODE_PIPE)
	{
		if (process_all_heredocs(ast->left, shell) == -1)
			return (-1);
		if (process_all_heredocs(ast->right, shell) == -1)
			return (-1);
	}
	else if (ast->type == NODE_COMMAND)
	{
		if (ast->heredocs)
		{
			if (process_heredocs(ast->heredocs, shell) == -1)
				return (-1);
		}
	}
	return (0);
}
