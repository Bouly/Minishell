/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_exec.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abendrih <abendrih@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 09:42:29 by abendrih          #+#    #+#             */
/*   Updated: 2025/11/02 16:41:25 by abendrih         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	simple_exec(t_token **token, char **envp)
{
	char	**args;
	char	*path;
	int		id;

	args = tokens_to_array(token);
	path = find_command(args[0], envp);
	// for (int i = 0; args[i]; i++)
	// 	ft_printf("%s\n", args[i]);
	if (path)
	{
		id = fork();
		if (id == 0)
		{
			execve(path, args, envp);
			perror("execve");
			ft_free(args);
			free(path);
			exit(1);
		}
		else
		{
			free(path);
			ft_free(args);
			wait(NULL);
		}
	}
	else
	{
		ft_free(args);
		ft_putstr_fd("command not found\n", 2);
	}
}
