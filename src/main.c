/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abendrih <abendrih@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 21:14:57 by abendrih          #+#    #+#             */
/*   Updated: 2025/11/12 20:13:26 by abendrih         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(int ac, char **av, char **env)
{
	char	*line;
	t_token	*token;
	t_ast	*three;
	t_shell	*shell;

	(void)ac;
	(void)av;
	shell = shell_init(env);
	if (!shell)
		return (ft_putstr_fd("Error: Failed to initialize shell\n", 2), 1);
	setup_signals_interactive();
	while (1)
	{
		line = readline("\033[1;91mEl Cancer > \033[0m");
		if (!line)
		{
			ft_putstr_fd("exit\n", 2);
			break ;
		}
		add_history(line);
		line = handle_multiline_input(line);
		token = lexer(line, shell);
		if (token == NULL)
		{
			free(line);
			continue ;
		}
		free(line);
		three = parse(token);
		token_free(&token);
		if (three)
		{
			mother_exec(three, shell, three);
			ast_free(&three);
		}
	}
	shell_free(shell);
	return (0);
}
