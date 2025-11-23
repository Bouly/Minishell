/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_process.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abendrih <abendrih@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 00:00:00 by abendrih          #+#    #+#             */
/*   Updated: 2025/11/23 11:14:44 by abendrih         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	read_heredoc_content(int fd, char *delim)
{
	char	*line;
	size_t	delim_len;
	size_t	line_len;

	delim_len = ft_strlen(delim);
	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		line_len = ft_strlen(line);
		if (line_len == delim_len && ft_strncmp(line, delim, delim_len) == 0)
		{
			free(line);
			break ;
		}
		write(fd, line, line_len);
		write(fd, "\n", 1);
		free(line);
	}
}

static int	process_single_heredoc(t_heredoc *heredoc, t_shell *shell)
{
	int		pipefd[2];
	pid_t	pid;
	int		status;

	if (pipe(pipefd) == -1)
		return (-1);
	pid = fork();
	if (pid == -1)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		return (-1);
	}
	if (pid == 0)
	{
		close(pipefd[0]);
		read_heredoc_content(pipefd[1], heredoc->delim);
		close(pipefd[1]);
		ast_free(&shell->ast);
		env_free(shell->env);
		exit(0);
	}
	close(pipefd[1]);
	waitpid(pid, &status, 0);
	heredoc->fd = pipefd[0];
	return (0);
}

static void	close_all_heredoc_fds(t_heredoc *heredocs)
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

int	process_heredocs(t_heredoc *heredocs, t_shell *shell)
{
	t_heredoc	*current;

	current = heredocs;
	while (current)
	{
		if (process_single_heredoc(current, shell) == -1)
		{
			close_all_heredoc_fds(heredocs);
			return (-1);
		}
		current = current->next;
	}
	return (0);
}

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
