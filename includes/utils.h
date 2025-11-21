/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abendrih <abendrih@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 19:00:00 by abendrih          #+#    #+#             */
/*   Updated: 2025/11/21 00:10:22 by abendrih         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

/* Input validation */
int		has_unclosed_quotes(char *line);
char	*handle_multiline_input(char *line);

#endif
