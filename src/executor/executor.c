/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abendrih <abendrih@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 16:29:04 by abendrih          #+#    #+#             */
/*   Updated: 2025/11/19 16:10:00 by abendrih         ###   ########.fr       */
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

void	cmd_exec(t_ast *node, char **envp, t_shell *shell)
{
	char	*path;
	int		id;
	int		status;

	if (is_builtin(node->args[0]))
		return (exec_builtin_with_redir(node, shell));
	path = find_command(node->args[0], envp);
	if (!path)
	{
		ft_putstr_fd("command not found\n", 2);
		shell->exit_status = 127;
		return ;
	}
	id = fork();
	if (id == 0)
		child_exec(node, path, envp);
	free(path);
	waitpid(id, &status, 0);
	shell->exit_status = get_exit_status(status);
}

void	mother_exec(t_ast *three, char **envp, t_ast *root, t_shell *shell)
{
	if (three->type == NODE_PIPE)
		pipe_exec(three, envp, root, shell);
	else if (three->type == NODE_COMMAND)
		cmd_exec(three, envp, shell);
}
