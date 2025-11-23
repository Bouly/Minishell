/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expension.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abendrih <abendrih@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 18:25:27 by abendrih          #+#    #+#             */
/*   Updated: 2025/11/23 12:46:14 by abendrih         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
** Extrait le nom de variable après un '$'
** Gère le cas spécial de '$?'
** Retourne: nom de la variable allouée
*/
static char	*get_var_name(char *str, int *i)
{
	int	start;

	start = *i;
	(*i)++;
	if (str[*i] == '?')
	{
		(*i)++;
		return (ft_strndup("?", 1));
	}
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	return (ft_strndup(&str[start + 1], *i - start - 1));
}

/*
** Récupère la valeur d'une variable d'environnement
** Gère '$?' qui retourne le code de sortie
** Retourne: valeur de la variable (chaîne vide si inexistante)
*/
static char	*get_var_value(char *var_name, t_env *env, int exit_status)
{
	char	*value;

	if (ft_strcmp(var_name, "?") == 0)
		return (ft_itoa(exit_status));
	value = env_get(env, var_name);
	if (!value)
		return (ft_strdup(""));
	return (ft_strdup(value));
}

/*
** Traite l'expansion d'une variable '$VAR'
** Extrait le nom et récupère sa valeur
** Retourne: valeur de la variable
*/
static char	*process_variable(char *str, int *i, t_env *env, int exit_status)
{
	char	*var_name;
	char	*var_value;

	var_name = get_var_name(str, i);
	var_value = get_var_value(var_name, env, exit_status);
	free(var_name);
	return (var_value);
}

/*
** Ajoute un caractère à la fin d'une chaîne
** Utilisé pour construire progressivement la chaîne expandée
** Retourne: nouvelle chaîne avec le caractère ajouté
*/
static char	*append_char(char *result, char c)
{
	char	*char_str;
	char	*new_result;

	char_str = ft_strndup(&c, 1);
	new_result = ft_strjoin(result, char_str);
	free(char_str);
	return (new_result);
}

/*
** Expanse toutes les variables d'environnement dans une chaîne
** Remplace $VAR par sa valeur, gère $?
** Retourne: nouvelle chaîne avec variables expansées
*/
char	*expand_variables(char *str, t_env *env, int exit_status)
{
	char	*result;
	char	*tmp;
	int		i;

	result = ft_strdup("");
	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && (ft_isalnum(str[i + 1]) || str[i + 1] == '_'
				|| str[i + 1] == '?'))
		{
			tmp = process_variable(str, &i, env, exit_status);
			result = ft_strjoin(result, tmp);
			free(tmp);
		}
		else
			result = append_char(result, str[i++]);
	}
	return (result);
}
