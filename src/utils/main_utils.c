/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahb <ahb@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 12:35:00 by abendrih          #+#    #+#             */
/*   Updated: 2025/11/23 16:44:22 by ahb              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
** Initialise la structure mother qui contient toutes les infos nécessaires
** à l'exécution des commandes (AST, environnement, référence au shell)
** Retourne: structure t_mother initialisée
*/
t_mother	init_mother(t_shell *shell)
{
	t_mother	mother;

	mother.root = shell->ast;
	mother.envp = env_to_array(shell->env);
	mother.shell = shell;
	shell->envp = mother.envp;
	return (mother);
}

/*
** Initialise le shell au démarrage du programme
** Configure l'environnement, le statut de sortie initial et les signaux
** Paramètres: shell - structure à initialiser, env - environnement système
*/
void	init_shell(t_shell *shell, char **env)
{
	shell->env = env_init(env);
	shell->exit_status = 0;
	shell->ast = NULL;
	shell->envp = NULL;
	setup_signals_interactive();
	enable_ctrl_chars_display();
}

/*
** Gère la fin de fichier (EOF / Ctrl-D)
** Affiche "exit", libère les ressources et quitte avec le code de sortie actuel
** Paramètres: shell - structure contenant l'état du shell
*/
void	handle_eof(t_shell *shell)
{
	int	exit_status;

	ft_putstr_fd("exit\n", 2);
	exit_status = shell->exit_status;
	env_free(shell->env);
	exit(exit_status);
}

/*
** Traite la ligne d'entrée utilisateur
** Gère les quotes non fermées, tokenise, parse et traite les heredocs
** Retourne: 1 si la commande est prête à être exécutée, 0 sinon
*/
int	process_input(char *line, t_shell *shell)
{
	t_token	*token;

	line = handle_multiline_input(line);
	token = lexer(line, shell);
	free(line);
	if (token == NULL)
		return (0);
	shell->ast = parse(token);
	if (!shell->ast)
	{
		token_free(&token);
		return (0);
	}
	token_free(&token);
	if (process_all_heredocs(shell->ast, shell) == -1)
	{
		if (shell->exit_status != 130)
		{
			ft_putstr_fd("heredoc error\n", 2);
			shell->exit_status = 1;
		}
		ast_free(&shell->ast);
		return (0);
	}
	return (1);
}

/*
** Exécute la commande contenue dans l'AST du shell
** Initialise mother, exécute et libère toutes les ressources après exécution
** Paramètres: shell - structure contenant l'AST à exécuter
*/
void	execute_command(t_shell *shell)
{
	t_mother	mother;

	mother = init_mother(shell);
	mother_exec(shell->ast, &mother);
	ft_free(shell->envp);
	ast_free(&shell->ast);
}
