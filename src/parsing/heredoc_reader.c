/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_reader.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abendrih <abendrih@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 17:30:00 by abendrih          #+#    #+#             */
/*   Updated: 2025/11/23 17:34:57 by abendrih         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
** Affiche un warning si le heredoc est terminé par EOF au lieu du delimiteur
** Libère les ressources (line, fd) si nécessaire
*/
void	handle_heredoc_eof(t_heredoc *heredoc, char *line, int fd)
{
	if (!line && !get_heredoc_interrupted())
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
}

/*
** Écrit une ligne dans le heredoc avec expansion optionnelle
** Si expand=1, les variables sont expandées avant écriture
*/
void	write_heredoc_line(int fd, char *line, t_heredoc *heredoc,
		t_shell *shell)
{
	char	*expanded;

	if (heredoc->expand)
	{
		expanded = expand_variables(line, shell->env, shell->exit_status);
		write(fd, expanded, ft_strlen(expanded));
		free(expanded);
	}
	else
		write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
}

/*
** Lit et écrit le contenu du heredoc avec expansion optionnelle
** Boucle readline jusqu'au delimiteur ou interruption (Ctrl-C)
** Paramètres: fd - où écrire, heredoc - infos heredoc, shell - pour expansion
*/
void	read_heredoc_content(int fd, t_heredoc *heredoc, t_shell *shell)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line || get_heredoc_interrupted())
		{
			handle_heredoc_eof(heredoc, line, fd);
			break ;
		}
		if (ft_strcmp(line, heredoc->delim) == 0)
		{
			free(line);
			break ;
		}
		write_heredoc_line(fd, line, heredoc, shell);
		free(line);
	}
}

/*
** Configure et exécute le processus fils du heredoc
** Configure les signaux, lit le contenu, nettoie et exit
*/
void	setup_heredoc_child(int pipefd[2], t_heredoc *heredoc, t_shell *shell)
{
	set_heredoc_interrupted(0);
	signal(SIGINT, heredoc_sigint_handler);
	close(pipefd[0]);
	read_heredoc_content(pipefd[1], heredoc, shell);
	close(pipefd[1]);
	ast_free(&shell->ast);
	env_free(shell->env);
	if (get_heredoc_interrupted())
		exit(130);
	exit(0);
}

/*
** Vérifie le statut de sortie du processus heredoc
** Détecte les interruptions (Ctrl-C) et met à jour exit_status
** Retourne: 0 si succès, -1 si interruption
*/
int	handle_heredoc_status(int status, int pipefd, t_shell *shell)
{
	if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
	{
		close(pipefd);
		shell->exit_status = 130;
		return (-1);
	}
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		close(pipefd);
		shell->exit_status = 130;
		return (-1);
	}
	return (0);
}
