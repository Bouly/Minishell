/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abendrih <abendrih@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 21:47:35 by abendrih          #+#    #+#             */
/*   Updated: 2025/11/12 17:01:23 by abendrih         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_WORD_SINGLE_QUOTED,
	TOKEN_WORD_DOUBLE_QUOTED,
	TOKEN_PIPE,
	TOKEN_REDIRECT_IN,
	TOKEN_REDIRECT_OUT,
	TOKEN_HEREDOC,
	TOKEN_APPEND,
}					t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
}					t_token;

t_token				*token_new(t_token_type type, char *value);
void				token_addback(t_token **lst, t_token *new);
int					count_tokens(t_token **lst);
void				token_free(t_token **lst);
char				**tokens_to_array(t_token **lst);
//--------------------------------------------------------------
int					is_space(char c);
int					is_operator(char c);
int					is_quote(char c);
int					skip_spaces(const char *line, int i);
char				*extract_quoted_word(char *line, int *i, char quote_char);
//-----------------------------------------------------------------
t_token				*lexer(char *line);

#endif
