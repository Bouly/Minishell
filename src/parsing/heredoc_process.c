/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_process.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahb <ahb@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 00:00:00 by abendrih          #+#    #+#             */
/*   Updated: 2025/11/23 16:44:22 by ahb              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static volatile sig_atomic_t	g_heredoc_interrupted = 0;

/*
** Gestionnaire de signal pour Ctrl-C dans le heredoc
** Nettoie les ressources et signale l'interruption
*/
static void	heredoc_sigint_handler(int sig)
{
	(void)sig;
	g_heredoc_interrupted = 1;
	rl_done = 1;
	write(STDOUT_FILENO, "\n", 1);
	close(STDIN_FILENO);
}

/*
** Lit et écrit le contenu du heredoc avec expansion optionnelle
** Paramètres: fd - où écrire, heredoc - infos heredoc, shell - pour expansion
*/
static void	read_heredoc_content(int fd, t_heredoc *heredoc, t_shell *shell)
{
	char	*line;
	char	*expanded;

	while (1)
	{
		line = readline("> ");
		if (!line || g_heredoc_interrupted)
		{
			if (!line && !g_heredoc_interrupted)
			{
				ft_putstr_fd("minishell: warning: here-document delimited by ", 2);
				ft_putstr_fd("end-of-file (wanted `", 2);
				ft_putstr_fd(heredoc->delim, 2);
				ft_putstr_fd("')\n", 2);
			}
			if (line)
				free(line);
			if (fd != -1)
				close(fd);
			break ;
		}
		if (ft_strcmp(line, heredoc->delim) == 0)
		{
			free(line);
			break ;
		}
		if (heredoc->expand)
		{
			expanded = expand_variables(line, shell->env, shell->exit_status);
			write(fd, expanded, ft_strlen(expanded));
			free(expanded);
		}
		else
			write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
}

/*
** Traite un heredoc : fork, lecture du contenu avec gestion signaux
** Crée un pipe pour stocker le contenu lu
** Retourne: 0 si succès, -1 si erreur ou interruption (Ctrl-C)
*/
static int	process_single_heredoc(t_heredoc *heredoc, t_shell *shell)
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
	{
		g_heredoc_interrupted = 0;
		signal(SIGINT, heredoc_sigint_handler);
		close(pipefd[0]);
		read_heredoc_content(pipefd[1], heredoc, shell);
		close(pipefd[1]);
		ast_free(&shell->ast);
		env_free(shell->env);
		if (g_heredoc_interrupted)
			exit(130);
		exit(0);
	}
	close(pipefd[1]);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
	{
		close(pipefd[0]);
		shell->exit_status = 130;
		return (-1);
	}
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		close(pipefd[0]);
		shell->exit_status = 130;
		return (-1);
	}
	return (heredoc->fd = pipefd[0], 0);
}

static void	close_all_heredoc_fds(t_heredoc *heredocs)
{
	t_heredoc	*current;

	current = heredocs;
	while (current)
	{
		if (current->fd != -1)
		{
			close(current->fd);
			current->fd = -1;
		}
		current = current->next;
	}
}

int	process_heredocs(t_heredoc *heredocs, t_shell *shell)
{
	t_heredoc	*current;

	current = heredocs;
	while (current)
	{
		if (process_single_heredoc(current, shell) == -1)
		{
			close_all_heredoc_fds(heredocs);
			return (-1);
		}
		current = current->next;
	}
	return (0);
}

int	process_all_heredocs(t_ast *ast, t_shell *shell)
{
	if (!ast)
		return (0);
	if (ast->type == NODE_PIPE)
	{
		if (process_all_heredocs(ast->left, shell) == -1)
			return (-1);
		if (process_all_heredocs(ast->right, shell) == -1)
			return (-1);
	}
	else if (ast->type == NODE_COMMAND)
	{
		if (ast->heredocs)
		{
			if (process_heredocs(ast->heredocs, shell) == -1)
				return (-1);
		}
	}
	return (0);
}
