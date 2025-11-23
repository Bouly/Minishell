/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abendrih <abendrih@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 04:18:54 by abendrih          #+#    #+#             */
/*   Updated: 2025/11/21 18:59:41 by abendrih         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
** Gère la tokenisation d'un pipe '|'
** Crée un token de type PIPE et avance l'index
** Retourne: 1 pour succès
*/
static int	handle_pipe(int *i, t_token **head)
{
	token_addback(head, token_new(TOKEN_PIPE, "|"));
	*i += 1;
	return (1);
}

/*
** Gère les redirections (<, >, <<, >>)
** Détecte le type de redirection et crée le token approprié
** Retourne: 1 pour succès
*/
static int	handle_redirect(const char *line, int *i, t_token **head)
{
	char	c;

	c = line[*i];
	if (c == '>')
	{
		if (line[*i + 1] == '>')
		{
			token_addback(head, token_new(TOKEN_APPEND, ">>"));
			*i += 2;
			return (1);
		}
		token_addback(head, token_new(TOKEN_REDIRECT_OUT, ">"));
		(*i)++;
		return (1);
	}
	if (line[*i + 1] == '<')
	{
		token_addback(head, token_new(TOKEN_HEREDOC, "<<"));
		*i += 2;
		return (1);
	}
	token_addback(head, token_new(TOKEN_REDIRECT_IN, "<"));
	(*i)++;
	return (1);
}

/*
** Accumule les parties adjacentes d'un mot (avec ou sans quotes)
** Gère la concaténation de mots collés comme abc"def"ghi
** Retourne: chaîne complète accumulée
*/
static char	*accumulate_adjacent_parts(char *line, int *i)
{
	char	*result;
	char	*part;
	int		start;

	result = ft_strdup("");
	while (line[*i] && !is_space(line[*i]) && !is_operator(line[*i]))
	{
		if (is_quote(line[*i]))
		{
			part = extract_quoted_word(line, i, line[*i]);
			if (!part)
				return (free(result), NULL);
		}
		else
		{
			start = *i;
			while (line[*i] && !is_space(line[*i]) && !is_operator(line[*i])
				&& !is_quote(line[*i]))
				(*i)++;
			part = ft_substr(line, start, *i - start);
		}
		result = ft_strjoin(result, part);
		free(part);
	}
	return (result);
}

/*
** Traite un mot (avec quotes ou non) et l'ajoute aux tokens
** Gère l'expansion de variables selon le type de quotes
** Retourne: 1 pour succès, 0 pour échec
*/
static int	handle_word(char *line, int *i, t_token **head, t_shell *shell)
{
	char			*word;
	char			*expanded;
	t_token_type	type;

	type = TOKEN_WORD;
	if (line[*i] == '\'')
		type = TOKEN_WORD_SINGLE_QUOTED;
	else if (line[*i] == '"')
		type = TOKEN_WORD_DOUBLE_QUOTED;
	word = accumulate_adjacent_parts(line, i);
	if (!word || word[0] == '\0')
	{
		free(word);
		return (1);
	}
	if (type == TOKEN_WORD_SINGLE_QUOTED)
		expanded = ft_strdup(word);
	else
		expanded = expand_variables(word, shell->env, shell->exit_status);
	free(word);
	token_addback(head, token_new(type, expanded));
	free(expanded);
	return (1);
}

/*
** Analyse lexicale de la ligne de commande
** Transforme la chaîne en liste de tokens (mots, opérateurs, redirections)
** Retourne: liste chaînée de tokens ou NULL si erreur
*/
t_token	*lexer(char *line, t_shell *shell)
{
	t_token	*head;
	int		i;
	int		ok;

	head = NULL;
	i = 0;
	while (line[i] != '\0')
	{
		i = skip_spaces(line, i);
		if (line[i] == '\0')
			break ;
		ok = 1;
		if (line[i] == '|')
			ok = handle_pipe(&i, &head);
		else if (line[i] == '>' || line[i] == '<')
			ok = handle_redirect(line, &i, &head);
		else
			ok = handle_word(line, &i, &head, shell);
		if (ok == 0)
			return (token_free(&head), NULL);
	}
	return (head);
}
