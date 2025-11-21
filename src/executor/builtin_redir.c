/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_redir.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abendrih <abendrih@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 17:30:00 by abendrih          #+#    #+#             */
/*   Updated: 2025/11/16 17:28:59 by abendrih         ###   ########.fr       */
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

int	setup_input_redirection(t_ast *node, int *saved_stdin)
{
	int			fd;
	t_heredoc	*last_heredoc;

	*saved_stdin = -1;
	last_heredoc = get_last_heredoc(node->heredocs);
	if (last_heredoc && last_heredoc->fd != -1)
	{
		*saved_stdin = dup(STDIN_FILENO);
		dup2(last_heredoc->fd, STDIN_FILENO);
		heredoc_close_all_fds(node->heredocs);
		return (1);
	}
	if (!node->infile)
		return (1);
	fd = open(node->infile, O_RDONLY);
	if (fd < 0)
	{
		perror("open infile");
		return (0);
	}
	*saved_stdin = dup(STDIN_FILENO);
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (1);
}

int	setup_output_redirection(t_ast *node, int *saved_stdout)
{
	int	fd;
	int	flags;

	*saved_stdout = -1;
	if (!node->outfile)
		return (1);
	if (node->append)
		flags = O_WRONLY | O_CREAT | O_APPEND;
	else
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	fd = open(node->outfile, flags, 0644);
	if (fd < 0)
	{
		perror("open outfile");
		return (0);
	}
	*saved_stdout = dup(STDOUT_FILENO);
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (1);
}

int	setup_builtin_redirections(t_ast *node, int saved_fd[2])
{
	if (!setup_input_redirection(node, &saved_fd[0]))
		return (0);
	if (!setup_output_redirection(node, &saved_fd[1]))
	{
		if (saved_fd[0] >= 0)
		{
			dup2(saved_fd[0], STDIN_FILENO);
			close(saved_fd[0]);
		}
		return (0);
	}
	return (1);
}

void	restore_redirections(int saved_fd[2])
{
	if (saved_fd[0] >= 0)
	{
		dup2(saved_fd[0], STDIN_FILENO);
		close(saved_fd[0]);
	}
	if (saved_fd[1] >= 0)
	{
		dup2(saved_fd[1], STDOUT_FILENO);
		close(saved_fd[1]);
	}
}

void	exec_builtin_with_redir(t_ast *node, t_shell *shell, t_ast *root)
{
	int	saved_fd[2];

	if (!setup_builtin_redirections(node, saved_fd))
	{
		shell->exit_status = 1;
		return ;
	}
	shell->exit_status = exec_builtin(node->args, shell);
	restore_redirections(saved_fd);
	close_all_ast_heredocs(root);
}
