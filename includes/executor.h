/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abendrih <abendrih@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 09:43:50 by abendrih          #+#    #+#             */
/*   Updated: 2025/11/16 17:28:59 by abendrih         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

typedef struct s_token	t_token;
typedef struct s_ast	t_ast;
typedef struct s_shell	t_shell;

/* executor.c - Main execution functions */
void					cmd_exec(t_ast *node, char **envp, t_shell *shell);
void					mother_exec(t_ast *three, char **envp, t_ast *root,
							t_shell *shell);

/* pipeline.c - Pipe execution */
void					pipe_exec(t_ast *three, char **envp, t_ast *root,
							t_shell *shell);

/* builtin_redir.c - Builtin with redirections */
int						setup_input_redirection(t_ast *node, int *saved_stdin);
int						setup_output_redirection(t_ast *node,
							int *saved_stdout);
int						setup_builtin_redirections(t_ast *node,
							int saved_fd[2]);
void					restore_redirections(int saved_fd[2]);
void					exec_builtin_with_redir(t_ast *node, t_shell *shell);

/* child_process.c - Child process execution */
void					child_exec(t_ast *node, char *path, char **envp);

/* Utils */
int						get_exit_status(int status);

#endif
