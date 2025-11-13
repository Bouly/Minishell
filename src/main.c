/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abendrih <abendrih@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 21:14:57 by abendrih          #+#    #+#             */
/*   Updated: 2025/11/13 20:09:16 by abendrih         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(int ac, char **av, char **env)
{
	char	*line;
	t_token	*token;
	t_ast	*three;

	(void)ac;
	(void)av;
	while (1)
	{
		line = readline("\033[1;91mEl Cancer > \033[0m");
		add_history(line);
		if (!line)
		{
			ft_putstr_fd("exit\n", 2);
			break ;
		}
		line = handle_multiline_input(line);
		token = lexer(line);
		free(line);
		if (token == NULL)
			continue ;
		three = parse(token);
		token_free(&token);
		mother_exec(three, env, three);
		ast_free(&three);
	}
}
