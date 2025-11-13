/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abendrih <abendrih@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 21:24:27 by abendrih          #+#    #+#             */
/*   Updated: 2025/11/12 20:04:31 by abendrih         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../Libft/libft.h"
# include "lexer.h"
# include "parser.h"

typedef struct s_shell
{
	char	**env;
	int		last_exit_status;
}	t_shell;

/* Shell initialization */
t_shell	*shell_init(char **envp);
void	shell_free(t_shell *shell);
char	**copy_env(char **envp);
char	*get_env_value(t_shell *shell, char *key);

# include "builtins.h"
# include "executor.h"
# include "signals.h"
# include "utils.h"

#endif
