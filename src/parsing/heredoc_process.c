/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_process.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abendrih <abendrih@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 00:00:00 by abendrih          #+#    #+#             */
/*   Updated: 2025/11/23 17:38:10 by abendrih         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static volatile sig_atomic_t	g_heredoc_interrupted = 0;

/*
** Gestionnaire de signal pour Ctrl-C dans le heredoc
** Nettoie les ressources et signale l'interruption
*/
void	heredoc_sigint_handler(int sig)
{
	(void)sig;
	g_heredoc_interrupted = 1;
	rl_done = 1;
	write(STDOUT_FILENO, "\n", 1);
	close(STDIN_FILENO);
}

int	get_heredoc_interrupted(void)
{
	return (g_heredoc_interrupted);
}

void	set_heredoc_interrupted(int value)
{
	g_heredoc_interrupted = value;
}

/*
** Traite un heredoc : fork, lecture du contenu avec gestion signaux
** Crée un pipe pour stocker le contenu lu
** Retourne: 0 si succès, -1 si erreur ou interruption (Ctrl-C)
*/
int	process_single_heredoc(t_heredoc *heredoc, t_shell *shell)
{
	int		pipefd[2];
	pid_t	pid;
	int		status;

	if (pipe(pipefd) == -1)
		return (-1);
	pid = fork();
	if (pid == -1)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		return (-1);
	}
	if (pid == 0)
		setup_heredoc_child(pipefd, heredoc, shell);
	close(pipefd[1]);
	waitpid(pid, &status, 0);
	if (handle_heredoc_status(status, pipefd[0], shell) == -1)
		return (-1);
	return (heredoc->fd = pipefd[0], 0);
}

int	process_heredocs(t_heredoc *heredocs, t_shell *shell)
{
	t_heredoc	*current;

	current = heredocs;
	while (current)
	{
		if (process_single_heredoc(current, shell) == -1)
		{
			heredoc_close_all_fds(heredocs);
			return (-1);
		}
		current = current->next;
	}
	return (0);
}
