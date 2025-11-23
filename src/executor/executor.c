/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abendrih <abendrih@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 16:29:04 by abendrih          #+#    #+#             */
/*   Updated: 2025/11/23 12:25:39 by abendrih         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
** Extrait le code de sortie d'un processus fils
** Gère les sorties normales et les signaux (Ctrl-C, etc)
** Retourne: code de sortie (0-255) ou 128+signal
*/
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

/*
** Exécute une commande simple (builtin ou externe)
** Gère les builtins avec redirections et les commandes PATH
** Paramètres: node - AST de la commande, mother - contexte
*/
void	cmd_exec(t_ast *node, t_mother *mother)
{
	char	*path;

	if (!node->args || !node->args[0])
		return ((void)handle_empty_command(node, mother));
	if (is_builtin(node->args[0]))
		return (exec_builtin_with_redir(node, mother));
	path = find_command(node->args[0], mother->envp);
	if (!path)
		return (handle_command_not_found(node->args[0], mother->shell));
	execute_external_command(node, path, mother);
}

/*
** Point d'entrée de l'exécution d'un nœud AST
** Détermine si c'est un pipe ou une commande simple
** Paramètres: three - AST, mother - contexte
*/
void	mother_exec(t_ast *three, t_mother *mother)
{
	if (three->type == NODE_PIPE)
		pipe_exec(three, mother);
	else if (three->type == NODE_COMMAND)
		cmd_exec(three, mother);
}
