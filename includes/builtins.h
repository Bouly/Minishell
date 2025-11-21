/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abendrih <abendrih@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 01:54:55 by aboulahd          #+#    #+#             */
/*   Updated: 2025/11/21 00:10:45 by abendrih         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# ifndef PATH_MAX
#  define PATH_MAX 4096
# endif

typedef struct s_ast	t_ast;

typedef struct s_env
{
	char				*key;
	char				*value;
	struct s_env		*next;
}						t_env;

typedef struct s_shell
{
	t_env				*env;
	int					exit_status;
	t_ast				*ast;
	char				**envp;
}						t_shell;
// Env
t_env					*env_init(char **envp);
void					env_free(t_env *env);
char					*env_get(t_env *env, const char *key);
int						env_set(t_env **env, const char *key,
							const char *value);
int						env_unset(t_env **env, const char *key);
char					**env_to_array(t_env *env);

// Builtins
int						builtin_pwd(char **args);
int						builtin_env(char **args, t_env *env);
int						builtin_echo(char **args);
int						builtin_cd(char **args, t_env **env);
int						builtin_export(char **args, t_env **env);
int						builtin_unset(char **args, t_env **env);
int						builtin_exit(char **args, t_shell *shell);

// Builtin detection
int						is_builtin(const char *cmd);
int						exec_builtin(char **args, t_shell *shell);

#endif
