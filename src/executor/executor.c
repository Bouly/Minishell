/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abendrih <abendrih@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 16:29:04 by abendrih          #+#    #+#             */
/*   Updated: 2025/11/12 18:54:14 by abendrih         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	cmd_exec(t_ast *node, char **envp)
{
	char	*path;
	int		id;
	int		fd;

	path = find_command(node->args[0], envp);
	if (!path)
	{
		ft_putstr_fd("command not found\n", 2);
		return ;
	}
	id = fork();
	if (id == 0)
	{
		if (node->infile)
		{
			fd = open(node->infile, O_RDONLY);
			if (fd < 0)
			{
				perror("open infile");
				exit(1);
			}
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		if (node->outfile)
		{
			if (node->append)
				fd = open(node->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
			else
				fd = open(node->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd < 0)
			{
				perror("open outfile");
				exit(1);
			}
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		execve(path, node->args, envp);
		perror("execve");
		free(path);
		exit(1);
	}
	free(path);
	wait(NULL);
}

void	mother_exec(t_ast *three, char **envp, t_ast *root)
{
	if (three->type == NODE_PIPE)
		pipe_exec(three, envp, root);
	else if (three->type == NODE_COMMAND)
		cmd_exec(three, envp);
}
