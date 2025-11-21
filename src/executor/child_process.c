/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abendrih <abendrih@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 17:30:00 by abendrih          #+#    #+#             */
/*   Updated: 2025/11/19 20:43:12 by abendrih         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static t_heredoc	*get_last_heredoc(t_heredoc *heredocs)
{
	t_heredoc	*last;

	if (!heredocs)
		return (NULL);
	last = heredocs;
	while (last->next)
		last = last->next;
	return (last);
}

static void	setup_child_input(t_ast *node)
{
	int			fd;
	t_heredoc	*last_heredoc;

	last_heredoc = get_last_heredoc(node->heredocs);
	if (last_heredoc && last_heredoc->fd != -1)
	{
		dup2(last_heredoc->fd, STDIN_FILENO);
		heredoc_close_all_fds(node->heredocs);
		return ;
	}
	if (!node->infile)
		return ;
	fd = open(node->infile, O_RDONLY);
	if (fd >= 0)
	{
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
}

static void	setup_child_output(t_ast *node)
{
	int	fd;
	int	flags;

	if (!node->outfile)
		return ;
	if (node->append)
		flags = O_WRONLY | O_CREAT | O_APPEND;
	else
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	fd = open(node->outfile, flags, 0644);
	if (fd >= 0)
	{
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
}

void	child_exec(t_ast *node, char *path, char **envp, t_ast *root)
{
	setup_signals_exec();
	setup_child_input(node);
	setup_child_output(node);
	close_all_ast_heredocs(root);
	execve(path, node->args, envp);
	perror("execve");
	free(path);
	ft_free(envp);
	exit(1);
}
