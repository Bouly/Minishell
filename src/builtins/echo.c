/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abendrih <abendrih@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 01:00:00 by abendrih          #+#    #+#             */
/*   Updated: 2025/11/21 00:10:39 by abendrih         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	builtin_echo(char **args)
{
	int	i;
	int	newline;

	newline = 1;
	i = 1;
	if (args[i] && ft_strcmp(args[i], "-n") == 0)
	{
		newline = 0;
		i++;
	}
	while (args[i])
	{
		ft_putstr_fd(args[i], 1);
		if (args[i + 1])
			ft_putstr_fd(" ", 1);
		i++;
	}
	if (newline)
		ft_putstr_fd("\n", 1);
	return (0);
}
