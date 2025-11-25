/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abendrih <abendrih@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 16:57:57 by abendrih          #+#    #+#             */
/*   Updated: 2025/11/25 21:07:04 by abendrih         ###   ########.fr       */
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
	int	fd;

	if (current->type == TOKEN_REDIRECT_OUT && current->next)
	{
		if (node->outfile)
		{
			fd = open(node->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd >= 0)
				close(fd);
			free(node->outfile);
		}
		node->outfile = ft_strdup(current->next->value);
		node->append = 0;
	}
}

static void	handle_append_redirect(t_token *current, t_ast *node)
{
	int	fd;

	if (current->type == TOKEN_APPEND && current->next)
	{
		if (node->outfile)
		{
			if (node->append)
				fd = open(node->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
			else
				fd = open(node->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd >= 0)
				close(fd);
			free(node->outfile);
		}
		node->outfile = ft_strdup(current->next->value);
		node->append = 1;
	}
}

static void	handle_heredoc(t_token *current, t_ast *node)
{
	t_heredoc	*new_heredoc;
	char		*delim;
	char		*clean_delim;
	size_t		len;

	if (current->type == TOKEN_HEREDOC && current->next)
	{
		delim = current->next->value;
		len = ft_strlen(delim);
		if (len >= 2 && ((delim[0] == '"' && delim[len - 1] == '"')
				|| (delim[0] == '\'' && delim[len - 1] == '\'')))
		{
			clean_delim = ft_substr(delim, 1, len - 2);
			new_heredoc = heredoc_new(clean_delim);
			new_heredoc->expand = 0;
			free(clean_delim);
		}
		else
		{
			new_heredoc = heredoc_new(delim);
			new_heredoc->expand = 1;
		}
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
