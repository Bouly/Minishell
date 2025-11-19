/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abendrih <abendrih@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 01:00:00 by abendrih          #+#    #+#             */
/*   Updated: 2025/11/19 19:12:30 by abendrih         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	builtin_env(char **args, t_env *env)
{
	// PWD=/home/user/minishell
	// SHLVL=1
	// _=/usr/bin/env

	(void)args;
	while (env)
	{
		ft_putstr_fd(env->key, 1);
		ft_putstr_fd("=", 1);
		ft_putstr_fd(env->value, 1);
		ft_putstr_fd("\n", 1);
		env = env->next;
	}
	return (0);
}

