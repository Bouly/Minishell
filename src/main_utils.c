/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abendrih <abendrih@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 12:35:00 by abendrih          #+#    #+#             */
/*   Updated: 2025/11/23 12:35:25 by abendrih         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_mother	init_mother(t_shell *shell)
{
	t_mother	mother;

	mother.root = shell->ast;
	mother.envp = env_to_array(shell->env);
	mother.shell = shell;
	shell->envp = mother.envp;
	return (mother);
}

void	init_shell(t_shell *shell, char **env)
{
	shell->env = env_init(env);
	shell->exit_status = 0;
	shell->ast = NULL;
	shell->envp = NULL;
	setup_signals_interactive();
	enable_ctrl_chars_display();
}

void	handle_eof(t_shell *shell)
{
	int	exit_status;

	ft_putstr_fd("exit\n", 2);
	exit_status = shell->exit_status;
	env_free(shell->env);
	exit(exit_status);
}

int	process_input(char *line, t_shell *shell)
{
	t_token	*token;

	line = handle_multiline_input(line);
	token = lexer(line, shell);
	free(line);
	if (token == NULL)
		return (0);
	shell->ast = parse(token);
	if (!shell->ast)
	{
		token_free(&token);
		return (0);
	}
	token_free(&token);
	if (process_all_heredocs(shell->ast, shell) == -1)
	{
		ft_putstr_fd("heredoc error\n", 2);
		shell->exit_status = 1;
		ast_free(&shell->ast);
		return (0);
	}
	return (1);
}

void	execute_command(t_shell *shell)
{
	t_mother	mother;

	mother = init_mother(shell);
	mother_exec(shell->ast, &mother);
	ft_free(shell->envp);
	ast_free(&shell->ast);
}
