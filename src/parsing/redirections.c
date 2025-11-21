/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abendrih <abendrih@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 16:57:57 by abendrih          #+#    #+#             */
/*   Updated: 2025/11/21 03:09:56 by abendrih         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	handle_input_redirect(t_token *current, t_ast *node)
{
	if (current->type == TOKEN_REDIRECT_IN && current->next)
	{
		if (node->infile)
			free(node->infile);
		node->infile = ft_strdup(current->next->value);
	}
}

static void	handle_output_redirect(t_token *current, t_ast *node)
{
	if (current->type == TOKEN_REDIRECT_OUT && current->next)
	{
		if (node->outfile)
			free(node->outfile);
		node->outfile = ft_strdup(current->next->value);
		node->append = 0;
	}
}

static void	handle_append_redirect(t_token *current, t_ast *node)
{
	if (current->type == TOKEN_APPEND && current->next)
	{
		if (node->outfile)
			free(node->outfile);
		node->outfile = ft_strdup(current->next->value);
		node->append = 1;
	}
}

static void	handle_heredoc(t_token *current, t_ast *node)
{
	t_heredoc	*new_heredoc;

	if (current->type == TOKEN_HEREDOC && current->next)
	{
		new_heredoc = heredoc_new(current->next->value);
		if (new_heredoc)
			heredoc_addback(&node->heredocs, new_heredoc);
	}
}

void	extract_redirections(t_token **tokens, t_ast *node)
{
	t_token	*current;

	current = *tokens;
	while (current)
	{
		if (current->type == TOKEN_REDIRECT_IN)
			handle_input_redirect(current, node);
		else if (current->type == TOKEN_REDIRECT_OUT)
			handle_output_redirect(current, node);
		else if (current->type == TOKEN_APPEND)
			handle_append_redirect(current, node);
		else if (current->type == TOKEN_HEREDOC)
			handle_heredoc(current, node);
		current = current->next;
	}
}
