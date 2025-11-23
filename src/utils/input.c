/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abendrih <abendrih@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 19:00:00 by abendrih          #+#    #+#             */
/*   Updated: 2025/11/14 01:56:30 by abendrih         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
** Vérifie si la ligne contient des quotes non fermées
** Compte les guillemets simples et doubles en ignorant ceux à l'intérieur
** Retourne: 1 si des quotes ne sont pas fermées, 0 sinon
*/
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

/*
** Gère l'entrée multiligne quand des quotes ne sont pas fermées
** Continue à lire des lignes avec le prompt ">" jusqu'à fermeture des quotes
** Retourne: la ligne complète avec toutes les lignes concaténées
*/
char	*handle_multiline_input(char *line)
{
	char	*next;
	char	*tmp;

	while (has_unclosed_quotes(line))
	{
		next = readline("> ");
		if (!next)
			return (line);
		add_history(next);
		tmp = ft_strjoin(line, "\n");
		line = ft_strjoin(tmp, next);
		free(next);
	}
	return (line);
}
