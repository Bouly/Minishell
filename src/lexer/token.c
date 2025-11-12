/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abendrih <abendrih@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 00:57:58 by abendrih          #+#    #+#             */
/*   Updated: 2025/11/12 20:04:30 by abendrih         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_token	*token_new(t_token_type type, char *value)
{
	t_token	*key;

	key = malloc(sizeof(t_token));
	if (!key)
		return (NULL);
	key->value = ft_strdup(value);
	key->type = type;
	key->next = NULL;
	return (key);
}

void	token_addback(t_token **lst, t_token *new)
{
	t_token	*key;

	if (!new)
		return ;
	if (!*lst)
	{
		*lst = new;
		new->next = NULL;
		return ;
	}
	key = *lst;
	while (key->next)
		key = key->next;
	key->next = new;
	new->next = NULL;
}

void	token_free(t_token **lst)
{
	t_token	*recip;

	if (!lst || !*lst)
		return ;
	while (*lst)
	{
		recip = *lst;
		*lst = (*lst)->next;
		free(recip->value);
		free(recip);
	}
	*lst = (NULL);
}
