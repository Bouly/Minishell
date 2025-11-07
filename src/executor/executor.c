/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abendrih <abendrih@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 16:29:04 by abendrih          #+#    #+#             */
/*   Updated: 2025/11/07 14:45:17 by abendrih         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	cmd_exec(char **args, char **envp)
{
	char	*path;
	int		id;

	path = find_command(args[0], envp);
	if (path)
	{
		id = fork();
		if (id == 0)
		{
			execve(path, args, envp);
			perror("execve");
			free(path);
			exit(1);
		}
		else
		{
			free(path);
			wait(NULL);
		}
	}
	else
	{
		ft_putstr_fd("command not found\n", 2);
	}
}

void	pipe_exec(t_ast *three, char **envp, t_ast *root)
{
	int	pid;
	int	pid2;
	int	fd[2];

	pipe(fd);
	pid = fork();
	if (pid == 0)
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		mother_exec(three->left, envp, root);
		ast_free(&root);
		exit(0);
	}
	pid2 = fork();
	if (pid2 == 0)
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		mother_exec(three->right, envp, root);
		ast_free(&root);
		exit(0);
	}
	close(fd[1]);
	close(fd[0]);
	waitpid(pid, NULL, 0);
	waitpid(pid2, NULL, 0);
}

void	mother_exec(t_ast *three, char **envp, t_ast *root)
{
	if (three->type == NODE_PIPE)
	{
		pipe_exec(three, envp, root);
	}
	else if (three->type == NODE_COMMAND)
	{
		cmd_exec(three->args, envp);
	}
}
