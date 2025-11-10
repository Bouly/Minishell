/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abendrih <abendrih@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 16:29:04 by abendrih          #+#    #+#             */
/*   Updated: 2025/11/09 18:20:17 by abendrih         ###   ########.fr       */
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
		ft_putstr_fd("command not found\n", 2);
}

void	mother_exec(t_ast *three, char **envp, t_ast *root)
{
	if (three->type == NODE_PIPE)
		pipe_exec(three, envp, root);
	else if (three->type == NODE_COMMAND)
		cmd_exec(three->args, envp);
}
