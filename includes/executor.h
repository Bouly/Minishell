/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abendrih <abendrih@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 09:43:50 by abendrih          #+#    #+#             */
/*   Updated: 2025/11/07 00:23:31 by abendrih         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

typedef struct s_token	t_token;
typedef struct s_ast	t_ast;
typedef struct s_shell	t_shell;

void					temporary_exec(t_token **token, char **envp);
void					pipe_exec(t_ast *three, char **envp, t_ast *root,
							t_shell *shell);
void					mother_exec(t_ast *three, char **envp, t_ast *root,
							t_shell *shell);

#endif
