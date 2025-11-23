/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abendrih <abendrih@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 21:14:57 by abendrih          #+#    #+#             */
/*   Updated: 2025/11/23 11:28:57 by abendrih         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(int ac, char **av, char **env)
{
	char	*line;
	t_token	*token;
	t_shell	shell;
	int		tmp;

	(void)ac;
	(void)av;
	if (isatty(STDIN_FILENO) == 0)
	{
		write(2, "\e[1;38;2;mSoit plus sympa sur tes tests ", 41);
		write(2, "stp et suis plutot la correction\n", 34);
		return (1);
	}
	shell.env = env_init(env);
	shell.exit_status = 0;
	shell.ast = NULL;
	tmp = 0;
	shell.envp = NULL;
	setup_signals_interactive();
	enable_ctrl_chars_display();
	while (1)
	{
		line = readline("\033[1;91mEl Cancer > \033[0m");
		if (!line)
		{
			ft_putstr_fd("exit\n", 2);
			tmp = shell.exit_status;
			env_free(shell.env);
			exit(tmp);
		}
		add_history(line);
		line = handle_multiline_input(line);
		token = lexer(line, &shell);
		free(line);
		if (token == NULL)
			continue ;
		shell.ast = parse(token);
		if (!shell.ast)
		{
			token_free(&token);
			continue ;
		}
		token_free(&token);
		if (process_all_heredocs(shell.ast, &shell) == -1)
		{
			ft_putstr_fd("heredoc error\n", 2);
			shell.exit_status = 1;
			ast_free(&shell.ast);
			continue ;
		}
		shell.envp = env_to_array(shell.env);
		mother_exec(shell.ast, shell.envp, shell.ast, &shell);
		ft_free(shell.envp);
		ast_free(&shell.ast);
	}
	env_free(shell.env);
}
