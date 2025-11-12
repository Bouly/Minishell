/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abendrih <abendrih@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 19:00:00 by abendrih          #+#    #+#             */
/*   Updated: 2025/11/12 20:13:22 by abendrih         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	has_unclosed_quotes(char *line)
{
	int	count;
	int	count_;
	int	i;

	count_ = 0;
	count = 0;
	i = 0;
	while (line[i])
	{
		if (line[i] == '"' && count_ % 2 == 0)
			count++;
		if (line[i] == '\'' && count % 2 == 0)
			count_++;
		i++;
	}
	return ((count + count_) % 2);
}

char	*handle_multiline_input(char *line)
{
	char	*next;
	char	*tmp;
	char	*old_line;

	while (has_unclosed_quotes(line))
	{
		next = readline("> ");
		if (!next)
			return (line);
		add_history(next);
		tmp = ft_strjoin(line, "\n");
		old_line = line;
		line = ft_strjoin(tmp, next);
		free(old_line);
		free(tmp);
		free(next);
	}
	return (line);
}
