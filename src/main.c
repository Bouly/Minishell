/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abendrih <abendrih@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 21:14:57 by abendrih          #+#    #+#             */
/*   Updated: 2025/11/19 19:20:48 by abendrih         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(int ac, char **av, char **env)
{
	char	*line;
	t_token	*token;
	t_ast	*three;
	t_shell	shell;
	char	**envp;

	(void)ac;
	(void)av;
	shell.env = env_init(env);
	shell.exit_status = 0;
	setup_signals_interactive();
	disable_ctrl_chars_display();
	while (1)
	{
		line = readline("\033[1;91mEl Cancer > \033[0m");
		if (!line)
		{
			ft_putstr_fd("exit\n", 2);
			break ;
		}
		add_history(line);
		if (ft_strcmp(line, "$?") == 0)
			printf("%d\n", shell.exit_status);
		line = handle_multiline_input(line);
		token = lexer(line);
		free(line);
		if (token == NULL)
			continue ;
		three = parse(token);
		token_free(&token);
		envp = env_to_array(shell.env);
		mother_exec(three, envp, three, &shell);
		ft_free(envp);
		ast_free(&three);
	}
	env_free(shell.env);
}
