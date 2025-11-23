/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abendrih <abendrih@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 19:00:00 by abendrih          #+#    #+#             */
/*   Updated: 2025/11/23 12:36:31 by abendrih         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

typedef struct s_shell	t_shell;
typedef struct s_mother	t_mother;

/* Input validation */
int						has_unclosed_quotes(char *line);
char					*handle_multiline_input(char *line);

/* Main utils */
t_mother				init_mother(t_shell *shell);
void					init_shell(t_shell *shell, char **env);
void					handle_eof(t_shell *shell);
int						process_input(char *line, t_shell *shell);
void					execute_command(t_shell *shell);

#endif
