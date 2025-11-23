/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abendrih <abendrih@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 16:29:04 by abendrih          #+#    #+#             */
/*   Updated: 2025/11/23 11:16:44 by abendrih         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	get_exit_status(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGQUIT)
			ft_putstr_fd("Quit (core dumped)\n", 2);
		return (128 + WTERMSIG(status));
	}
	return (0);
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

void	cmd_exec(t_ast *node, char **envp, t_shell *shell, t_ast *root)
{
	char	*path;
	int		id;
	int		status;
	int		fd;
	int		flags;

	if (!node->args || !node->args[0])
	{
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
				shell->exit_status = 1;
				return ;
			}
			close(fd);
			shell->exit_status = 0;
			close_all_ast_heredocs(root);
			return ;
		}
		ft_putstr_fd("syntax error: no command\n", 2);
		shell->exit_status = 2;
		return ;
	}
	if (is_builtin(node->args[0]))
		return (exec_builtin_with_redir(node, shell, root));
	path = find_command(node->args[0], envp);
	if (!path)
	{
		ft_putstr_fd("command not found\n", 2);
		shell->exit_status = 127;
		return ;
	}
	setup_signals_child();
	id = fork();
	if (id == 0)
		child_exec(node, path, envp, root);
	free(path);
	waitpid(id, &status, 0);
	setup_signals_interactive();
	shell->exit_status = get_exit_status(status);
}

void	mother_exec(t_ast *three, char **envp, t_ast *root, t_shell *shell)
{
	if (three->type == NODE_PIPE)
		pipe_exec(three, envp, root, shell);
	else if (three->type == NODE_COMMAND)
		cmd_exec(three, envp, shell, root);
}
