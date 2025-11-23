/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abendrih <abendrih@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 00:00:00 by abendrih          #+#    #+#             */
/*   Updated: 2025/11/23 11:14:44 by abendrih         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_heredoc	*heredoc_new(char *delim)
{
	t_heredoc	*new;

	new = malloc(sizeof(t_heredoc));
	if (!new)
		return (NULL);
	new->delim = ft_strdup(delim);
	new->fd = -1;
	new->next = NULL;
	return (new);
}

void	heredoc_addback(t_heredoc **lst, t_heredoc *new)
{
	t_heredoc	*current;

	if (!lst || !new)
		return ;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	current = *lst;
	while (current->next)
		current = current->next;
	current->next = new;
}

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
