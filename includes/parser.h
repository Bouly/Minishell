/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abendrih <abendrih@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 09:30:21 by abendrih          #+#    #+#             */
/*   Updated: 2025/11/23 17:29:20 by abendrih         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

typedef enum e_node_type
{
	NODE_COMMAND,
	NODE_PIPE,
}						t_node_type;

typedef struct s_heredoc
{
	char				*delim;
	int					fd;
	int					expand;
	struct s_heredoc	*next;
}						t_heredoc;

typedef struct s_ast
{
	t_node_type			type;
	char				**args;
	char				*infile;
	char				*outfile;
	int					append;
	t_heredoc			*heredocs;
	struct s_ast		*left;
	struct s_ast		*right;
}						t_ast;

/* AST management */
t_ast					*create_ast(t_node_type type, char **args);
void					ast_free(t_ast **tree);

/* Heredoc management */
t_heredoc				*heredoc_new(char *delim);
void					heredoc_addback(t_heredoc **lst, t_heredoc *new);
void					heredoc_free(t_heredoc **lst);
void					heredoc_close_all_fds(t_heredoc *heredocs);
void					close_all_ast_heredocs(t_ast *ast);
int						process_heredocs(t_heredoc *heredocs, t_shell *shell);
int						process_all_heredocs(t_ast *ast, t_shell *shell);
int						process_single_heredoc(t_heredoc *heredoc,
							t_shell *shell);

/* Heredoc signal handling */
void					heredoc_sigint_handler(int sig);
int						get_heredoc_interrupted(void);
void					set_heredoc_interrupted(int value);

/* Heredoc reader functions */
void					handle_heredoc_eof(t_heredoc *heredoc, char *line,
							int fd);
void					write_heredoc_line(int fd, char *line,
							t_heredoc *heredoc, t_shell *shell);
void					read_heredoc_content(int fd, t_heredoc *heredoc,
							t_shell *shell);
void					setup_heredoc_child(int pipefd[2], t_heredoc *heredoc,
							t_shell *shell);
int						handle_heredoc_status(int status, int pipefd,
							t_shell *shell);

/* Parser */
t_ast					*parse(t_token *tokens);
t_token					*find_pipe(t_token *tokens);

/* Redirections */
void					extract_redirections(t_token **tokens, t_ast *node);

/* Utils */
char					**tokens_to_array(t_token **lst);
char					*get_path_from_env(char **envp);
char					*find_command(char *cmd, char **envp);

#endif
