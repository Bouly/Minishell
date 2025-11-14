/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboulahd <aboulahd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 16:29:04 by abendrih          #+#    #+#             */
/*   Updated: 2025/11/14 02:47:42 by aboulahd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	setup_redirections(t_ast *node, int saved_fd[2])
{
	int	fd;

	saved_fd[0] = -1;
	saved_fd[1] = -1;
	if (node->infile && ((fd = open(node->infile, O_RDONLY)) < 0))
		return (perror("open infile"), 0);
	if (node->infile)
		(saved_fd[0] = dup(STDIN_FILENO), dup2(fd, STDIN_FILENO), close(fd));
	if (node->outfile)
	{
		if (node->append)
			fd = open(node->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			fd = open(node->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd < 0)
			return (perror("open outfile"), saved_fd[0] >= 0
				&& (dup2(saved_fd[0], STDIN_FILENO), close(saved_fd[0])), 0);
		saved_fd[1] = dup(STDOUT_FILENO);
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	return (1);
}

static void	exec_builtin_with_redir(t_ast *node, t_shell *shell)
{
	int	saved_fd[2];

	if (!setup_redirections(node, saved_fd))
	{
		shell->exit_status = 1;
		return ;
	}
	shell->exit_status = exec_builtin(node->args, shell);
	if (saved_fd[0] >= 0)
		(dup2(saved_fd[0], STDIN_FILENO), close(saved_fd[0]));
	if (saved_fd[1] >= 0)
		(dup2(saved_fd[1], STDOUT_FILENO), close(saved_fd[1]));
}

static void	child_exec(t_ast *node, char *path, char **envp)
{
	int	fd;

	if (node->infile)
	{
		fd = open(node->infile, O_RDONLY);
		if (fd >= 0)
			(dup2(fd, STDIN_FILENO), close(fd));
	}
	if (node->outfile)
	{
		if (node->append)
			fd = open(node->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			fd = open(node->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd >= 0)
			(dup2(fd, STDOUT_FILENO), close(fd));
	}
	execve(path, node->args, envp);
	perror("execve");
	free(path);
	exit(1);
}

void	cmd_exec(t_ast *node, char **envp, t_shell *shell)
{
	char	*path;
	int		id;


	if (is_builtin(node->args[0]))
		return (exec_builtin_with_redir(node, shell));
	path = find_command(node->args[0], envp);
	if (!path)
		return (ft_putstr_fd("command not found\n", 2),
			shell->exit_status = 127, (void)0);
	id = fork();
	if (id == 0)
		child_exec(node, path, envp);
	free(path);
	wait(NULL);
	shell->exit_status = 0;
}

void	mother_exec(t_ast *three, char **envp, t_ast *root, t_shell *shell)
{
	if (three->type == NODE_PIPE)
		pipe_exec(three, envp, root, shell);
	else if (three->type == NODE_COMMAND)
		cmd_exec(three, envp, shell);
}
