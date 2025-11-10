/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abendrih <abendrih@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 09:30:21 by abendrih          #+#    #+#             */
/*   Updated: 2025/11/10 17:12:43 by abendrih         ###   ########.fr       */
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
	struct s_ast	*left;
	struct s_ast	*right;
}					t_ast;

char				*get_path_from_env(char **envp);
char				*find_command(char *cmd, char **envp);
t_token				*find_pipe(t_token *tokens);
void				ast_free(t_ast **three);

t_ast				*parse(t_token *tokens);

#endif
