/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abendrih <abendrih@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 17:10:10 by abendrih          #+#    #+#             */
/*   Updated: 2025/11/23 20:25:48 by abendrih         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
** Configure les file descriptors pour un côté du pipe
** side=1 pour écriture (gauche), side=0 pour lecture (droite)
** Fait le dup2 et ferme les fds du pipe
*/
static void	setup_pipe_fd(int fd[2], int side)
{
	if (side == 1)
		dup2(fd[1], STDOUT_FILENO);
	else
		dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
	close(fd[1]);
}

/*
** Code exécuté par le processus fils d'un pipe
** Configure les fds, exécute la commande et nettoie
*/
static void	execute_pipe_child(t_ast *node, int fd[2], int side,
		t_mother *mother)
{
	setup_signals_exec();
	setup_pipe_fd(fd, side);
	mother_exec(node, mother);
	ast_free(&mother->root);
	ft_free(mother->envp);
	env_free(mother->shell->env);
	exit(0);
}

/*
** Exécute un pipeline (commande | commande)
** Fork 2 processus, configure le pipe et attend la fin
** Paramètres: three - nœud PIPE, mother - contexte
*/
void	pipe_exec(t_ast *three, t_mother *mother)
{
	int	pid;
	int	pid2;
	int	fd[2];
	int	status;

	status = 0;
	pipe(fd);
	setup_signals_child();
	pid = fork();
	if (pid == 0)
		execute_pipe_child(three->left, fd, 1, mother);
	pid2 = fork();
	if (pid2 == 0)
		execute_pipe_child(three->right, fd, 0, mother);
	close(fd[0]);
	close(fd[1]);
	waitpid(pid, NULL, 0);
	waitpid(pid2, &status, 0);
	setup_signals_interactive();
	mother->shell->exit_status = get_exit_status(status);
}
