/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herdoc_ultils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abendrih <abendrih@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 17:34:08 by abendrih          #+#    #+#             */
/*   Updated: 2025/11/23 17:36:01 by abendrih         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
** Crée un nouveau heredoc avec son délimiteur
** Initialise le fd à -1 (pas encore ouvert)
** Retourne: nouveau heredoc alloué
*/
t_heredoc	*heredoc_new(char *delim)
{
	t_heredoc	*new;

	new = malloc(sizeof(t_heredoc));
	if (!new)
		return (NULL);
	new->delim = ft_strdup(delim);
	new->fd = -1;
	new->expand = 1;
	new->next = NULL;
	return (new);
}

/*
** Ajoute un heredoc à la fin de la liste
** Paramètres: lst - liste, new - heredoc à ajouter
*/
void	heredoc_addback(t_heredoc **lst, t_heredoc *new)
{
	t_heredoc	*current;

	if (!lst || !new)
		return ;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	current = *lst;
	while (current->next)
		current = current->next;
	current->next = new;
}
