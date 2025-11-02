/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abendrih <abendrih@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 00:57:58 by abendrih          #+#    #+#             */
/*   Updated: 2025/11/02 16:48:55 by abendrih         ###   ########.fr       */
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

int	count_tokens(t_token **lst)
{
	int		i;
	t_token	*key;

	i = 0;
	key = *lst;
	while (key)
	{
		key = key->next;
		i++;
	}
	return (i);
}

char	**tokens_to_array(t_token **lst)
{
	int		i;
	char	**args;

	i = 0;
	args = malloc(sizeof(char *) * (count_tokens(lst) + 1));
	if (!args)
		return (NULL);
	while (*lst)
	{
		if ((*lst)->type != TOKEN_WORD
			&& (*lst)->type != TOKEN_WORD_DOUBLE_QUOTED
			&& (*lst)->type != TOKEN_WORD_SINGLE_QUOTED)
		{
			args[i] = NULL;
			if ((*lst)->next)
				*lst = (*lst)->next;
			return (args);
		}
		args[i] = ft_strdup((*lst)->value);
		i++;
		*lst = (*lst)->next;
	}
	args[i] = NULL;
	return (args);
}
