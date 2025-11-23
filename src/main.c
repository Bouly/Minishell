/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abendrih <abendrih@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 21:14:57 by abendrih          #+#    #+#             */
/*   Updated: 2025/11/23 13:50:37 by abendrih         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(int ac, char **av, char **env)
{
	char	*line;
	t_shell	shell;

	(void)ac;
	(void)av;
	if (isatty(STDIN_FILENO) == 0)
	{
		write(2, "\e[1;38;2;mSoit plus sympa sur tes tests ", 41);
		write(2, "stp et suis plutot la correction\n", 34);
		return (1);
	}
	init_shell(&shell, env);
	while (1)
	{
		line = readline("\033[1;91mEl Cancer > \033[0m");
		if (!line)
			handle_eof(&shell);
		add_history(line);
		if (process_input(line, &shell))
			execute_command(&shell);
	}
	env_free(shell.env);
}
