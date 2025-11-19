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

static void	setup_child_input(t_ast *node)
{
	int	fd;

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

void	child_exec(t_ast *node, char *path, char **envp)
{
	setup_signals_exec();
	setup_child_input(node);
	setup_child_output(node);
	execve(path, node->args, envp);
	perror("execve");
	free(path);
	ft_free(envp);
	exit(1);
}
