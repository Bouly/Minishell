/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abendrih <abendrih@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 01:00:00 by abendrih          #+#    #+#             */
/*   Updated: 2025/11/21 03:18:31 by abendrih         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	builtin_env(char **args, t_env *env)
{
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

/*implementer si lenv est supprimer ou quil ne le trouve pas juste imprimer ca
	PWD=/home/user/minishell
	SHLVL=1
	_=/usr/bin/env
*/
