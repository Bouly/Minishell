/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abendrih <abendrih@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 21:14:57 by abendrih          #+#    #+#             */
/*   Updated: 2025/11/12 16:24:03 by abendrih         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	has_unclosed_quotes(char *line)
{
	int	count;
	int	count_;
	int	i;

	count_ = 0;
	count = 0;
	i = 0;
	while (line[i])
	{
		if (line[i] == '"' && count_ % 2 == 0)
			count++;
		if (line[i] == '\'' && count % 2 == 0)
			count_++;
		i++;
	}
	return ((count + count_) % 2);
}

int	main(int ac, char **av, char **env)
{
	char	*line;
	t_token	*token;
	t_ast	*three;
	char	*next;
	char	*tmp;

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
		while (has_unclosed_quotes(line))
		{
			next = readline("> ");
			add_history(line);
			tmp = ft_strjoin(line, "\n");
			line = ft_strjoin(tmp, next);
			free(next);
		}
		token = lexer(line);
		if (token == NULL)
		{
			free(line);
			continue ;
		}
		free(line);
		three = parse(token);
		token_free(&token);
		mother_exec(three, env, three);
		ast_free(&three);
	}
}
