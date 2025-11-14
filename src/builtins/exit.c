/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abendrih <abendrih@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 01:00:00 by abendrih          #+#    #+#             */
/*   Updated: 2025/11/14 01:00:00 by abendrih         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	is_numeric(const char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	builtin_exit(char **args, t_shell *shell)
{
	int	exit_code;

	ft_putstr_fd("exit\n", 2);
	if (args[1])
	{
		if (!is_numeric(args[1]))
		{
			ft_putstr_fd("exit: numeric argument required\n", 2);
			env_free(shell->env);
			exit(255);
		}
		if (args[2])
		{
			ft_putstr_fd("exit: too many arguments\n", 2);
			return (1);
		}
		exit_code = ft_atoi(args[1]);
		env_free(shell->env);
		exit(exit_code & 255);
	}
	env_free(shell->env);
	exit(shell->exit_status);
}
