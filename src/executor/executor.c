/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abendrih <abendrih@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 16:29:04 by abendrih          #+#    #+#             */
/*   Updated: 2025/11/16 17:28:59 by abendrih         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	cmd_exec(t_ast *node, char **envp, t_shell *shell)
{
	char	*path;
	int		id;

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
