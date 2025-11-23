/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abendrih <abendrih@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 21:14:57 by abendrih          #+#    #+#             */
/*   Updated: 2025/11/23 20:22:17 by abendrih         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
** Fonction principale du minishell
** Initialise le shell, configure les signaux et lance la boucle de lecture
** des commandes. Vérifie d'abord que l'entrée est un terminal (TTY).
** Paramètres: env - tableau des variables d'environnement du système
*/
int	main(int ac, char **av, char **env)
{
	char	*line;
	t_shell	shell;

	(void)ac;
	(void)av;
	if (isatty(STDIN_FILENO) == 0)
		return (ft_putstr_fd("\e[1;38;2;mstp suis plutot la correction\n", 2),
			1);
	init_shell(&shell, env);
	while (1)
	{
		line = readline("\001\033[1;91m\002El Cancer > \001\033[0m\002");
		if (g_signal)
		{
			shell.exit_status = g_signal;
			g_signal = 0;
		}
		if (!line)
			handle_eof(&shell);
		add_history(line);
		if (process_input(line, &shell))
			execute_command(&shell);
	}
	env_free(shell.env);
}
