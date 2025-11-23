/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abendrih <abendrih@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 12:20:00 by abendrih          #+#    #+#             */
/*   Updated: 2025/11/23 20:25:48 by abendrih         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	handle_empty_command(t_ast *node, t_mother *mother)
{
	int	fd;
	int	flags;

	if (node->outfile)
	{
		if (node->append)
			flags = O_WRONLY | O_CREAT | O_APPEND;
		else
			flags = O_WRONLY | O_CREAT | O_TRUNC;
		fd = open(node->outfile, flags, 0644);
		if (fd < 0)
		{
			perror("open");
			mother->shell->exit_status = 1;
			return (0);
		}
		close(fd);
		mother->shell->exit_status = 0;
		close_all_ast_heredocs(mother->root);
		return (0);
	}
	ft_putstr_fd("syntax error: no command\n", 2);
	mother->shell->exit_status = 2;
	return (0);
}

void	handle_command_not_found(char *cmd, t_shell *shell)
{
	if (ft_strchr(cmd, '/'))
	{
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": ", 2);
		if (access(cmd, F_OK) == 0)
		{
			ft_putstr_fd("Is a directory\n", 2);
			shell->exit_status = 126;
		}
		else
		{
			ft_putstr_fd("No such file or directory\n", 2);
			shell->exit_status = 127;
		}
	}
	else
	{
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": command not found\n", 2);
		shell->exit_status = 127;
	}
}

void	execute_external_command(t_ast *node, char *path, t_mother *mother)
{
	int	id;
	int	status;

	status = 0;
	setup_signals_child();
	id = fork();
	if (id == 0)
		child_exec(node, path, mother);
	free(path);
	waitpid(id, &status, 0);
	setup_signals_interactive();
	mother->shell->exit_status = get_exit_status(status);
}

void	exec_builtin_with_redir(t_ast *node, t_mother *mother)
{
	int	saved_fd[2];

	if (!setup_builtin_redirections(node, saved_fd))
	{
		mother->shell->exit_status = 1;
		return ;
	}
	mother->shell->exit_status = exec_builtin(node->args, mother->shell);
	restore_redirections(saved_fd);
	close_all_ast_heredocs(mother->root);
}
