/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abendrih <abendrih@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 20:32:59 by abendrih          #+#    #+#             */
/*   Updated: 2025/08/12 23:29:30 by abendrih         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

int	ft_lstsize(t_stack_node *lst)
{
	int				i;
	t_stack_node	*key;

	i = 0;
	key = lst;
	while (key)
	{
		key = key->next;
		i++;
	}
	return (i);
}
