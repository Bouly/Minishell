/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abendrih <abendrih@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 16:29:04 by abendrih          #+#    #+#             */
/*   Updated: 2025/11/12 20:08:35 by abendrih         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	setup_heredoc(char *delimiter)
{
	char	*line;
	int		pipe_fd[2];

	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		exit(1);
	}
	while (1)
	{
		line = readline("> ");
		if (!line || (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0
				&& ft_strlen(line) == ft_strlen(delimiter)))
		{
			free(line);
			break ;
		}
		write(pipe_fd[1], line, ft_strlen(line));
		write(pipe_fd[1], "\n", 1);
		free(line);
	}
	close(pipe_fd[1]);
	dup2(pipe_fd[0], STDIN_FILENO);
	close(pipe_fd[0]);
}

static void	setup_infile(char *infile)
{
	int	fd;

	fd = open(infile, O_RDONLY);
	if (fd < 0)
	{
		perror("open infile");
		exit(1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
}

static void	setup_outfile(char *outfile, int append)
{
	int	fd;

	if (append)
		fd = open(outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror("open outfile");
		exit(1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
}

void	cmd_exec(t_ast *node, t_shell *shell)
{
	char	*path;
	int		id;
	int		status;

	path = find_command(node->args[0], shell->env);
	if (!path)
	{
		ft_putstr_fd("command not found\n", 2);
		shell->last_exit_status = 127;
		return ;
	}
	id = fork();
	if (id == 0)
	{
		if (node->heredoc_delim)
			setup_heredoc(node->heredoc_delim);
		else if (node->infile)
			setup_infile(node->infile);
		if (node->outfile)
			setup_outfile(node->outfile, node->append);
		execve(path, node->args, shell->env);
		perror("execve");
		free(path);
		exit(1);
	}
	free(path);
	waitpid(id, &status, 0);
	if (WIFEXITED(status))
		shell->last_exit_status = WEXITSTATUS(status);
}

void	mother_exec(t_ast *three, t_shell *shell, t_ast *root)
{
	if (three->type == NODE_PIPE)
		pipe_exec(three, shell, root);
	else if (three->type == NODE_COMMAND)
		cmd_exec(three, shell);
}
