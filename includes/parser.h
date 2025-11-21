/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abendrih <abendrih@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 09:30:21 by abendrih          #+#    #+#             */
/*   Updated: 2025/11/21 00:10:17 by abendrih         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

typedef enum e_node_type
{
	NODE_COMMAND,
	NODE_PIPE,
}					t_node_type;

typedef struct s_ast
{
	t_node_type		type;
	char			**args;
	char			*infile;
	char			*outfile;
	int				append;
	char			*heredoc_delim;
	struct s_ast	*left;
	struct s_ast	*right;
}					t_ast;

/* AST management */
t_ast				*create_ast(t_node_type type, char **args);
void				ast_free(t_ast **tree);

/* Parser */
t_ast				*parse(t_token *tokens);
t_token				*find_pipe(t_token *tokens);

/* Redirections */
void				extract_redirections(t_token **tokens, t_ast *node);

/* Utils */
char				**tokens_to_array(t_token **lst);
char				*get_path_from_env(char **envp);
char				*find_command(char *cmd, char **envp);

#endif
