/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abendrih <abendrih@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 17:10:10 by abendrih          #+#    #+#             */
/*   Updated: 2025/11/12 20:06:06 by abendrih         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	close_fd(int fd[2], int n)
{
	if (n == 1)
		dup2(fd[1], STDOUT_FILENO);
	else
		dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
	close(fd[1]);
}

void	pipe_exec(t_ast *three, t_shell *shell, t_ast *root)
{
	int	pid;
	int	pid2;
	int	fd[2];
	int	status;

	pipe(fd);
	pid = fork();
	if (pid == 0)
	{
		close_fd(fd, 1);
		mother_exec(three->left, shell, root);
		ast_free(&root);
		exit(shell->last_exit_status);
	}
	pid2 = fork();
	if (pid2 == 0)
	{
		close_fd(fd, 0);
		mother_exec(three->right, shell, root);
		ast_free(&root);
		exit(shell->last_exit_status);
	}
	close(fd[0]);
	close(fd[1]);
	waitpid(pid, NULL, 0);
	waitpid(pid2, &status, 0);
	if (WIFEXITED(status))
		shell->last_exit_status = WEXITSTATUS(status);
}
