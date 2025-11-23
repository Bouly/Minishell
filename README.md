Brouillon

# Minishell 

> *Un interpréteur de commandes simplifié, reproduisant le comportement de bash*

[![42 Project](https://img.shields.io/badge/42-Project-00babc?style=flat-square&logo=42)](https://42.fr)
[![Norminette](https://img.shields.io/badge/Norminette-passing-success?style=flat-square)](https://github.com/42School/norminette)
[![Memory Leaks](https://img.shields.io/badge/Memory%20Leaks-0-success?style=flat-square)]()

---

## L'équipe

Ce projet a été développé en binôme dans le cadre du cursus 42

<div style="display: flex; gap: 20px;">
  <table>
    <tr>
      <td align="center">
        <img src="https://img.shields.io/badge/abendrih-blue?style=for-the-badge" alt="abendrih"/>
        <br />
        <sub><b>Architecture & Core</b></sub>
        <br />
        <sub>Lexer • Parser • Executor</sub>
        <br />
        <sub>Variable Expansion • Memory Management</sub>
      </td>
      <td align="center">
        <img src="https://img.shields.io/badge/aboulahd-green?style=for-the-badge" alt="aboulahd"/>
        <br />
        <sub><b>Builtins & System</b></sub>
        <br />
        <sub>7 Builtins • Environment • Signals</sub>
        <br />
        <sub>Testing • Heredoc (WIP)</sub>
      </td>
    </tr>
  </table>
  <img width="300" src="https://github.com/user-attachments/assets/87cbd632-0f63-4419-9f79-faf13d66b99f" alt="Diagram"/>
</div>

---


## Description

**Minishell** est un projet du cursus 42 qui consiste à créer un shell minimal en C. L'objectif est de comprendre en profondeur le fonctionnement d'un interpréteur de commandes, la gestion des processus et les mécanismes Unix.

Notre implémentation reproduit les fonctionnalités essentielles de bash avec :
- Une architecture propre et modulaire
- Une gestion mémoire rigoureuse (0 leaks)
- Un code conforme à la Norminette
<img width="200" height="200" alt="image" src="https://github.com/user-attachments/assets/7426a7b7-eb31-47ea-8e95-7163f7b00913" />


---

## Fonctionnalités

### Implémenté

- **7 Builtins** : `echo` (avec -n), `cd`, `pwd`, `export`, `unset`, `env`, `exit`
- **Variables** : `$VAR` et `$?` (exit status)
- **Quotes** : Simple `'...'` et double `"..."` avec expansion
- **Redirections** : `<` (input), `>` (output), `>>` (append)
- **Pipes** : Support des pipes simples et multiples (`|`)
- **Signaux** : Gestion de Ctrl+C et Ctrl+\\
- **Chemins absolus** : `/bin/echo`, `./script`, etc.
- **Multiline input** : Gestion des quotes non fermées

<img width="200" height="200" alt="image" src="https://github.com/user-attachments/assets/56cc9a6b-9861-4193-b6a5-64228625c453" />


---

## Utilisation

### Installation

```bash
git clone https://github.com/AnisseBendrihem/Minishell.git
cd minishell
make
```

### Lancement

```bash
./minishell
```

### Exemples

```bash
# Commandes simples
echo "Hello, World!"
pwd
ls -la

# Variables
export NAME=Minishell
echo "Welcome to $NAME"
echo $?

# Pipes
cat file.txt | grep "pattern" | wc -l

# Redirections
echo "test" > output.txt
cat < input.txt
echo "append" >> output.txt

# Builtins
cd /tmp
pwd
export VAR=value
unset VAR
env | grep PATH
exit 0
```

<img width="200" height="200" alt="image" src="https://github.com/user-attachments/assets/8c93f4da-72e2-4800-99fc-4dfa1f1dd08f" />

---

### Points clés de l'architecture

1. **Lexer avec expansion** : Les variables sont expand pendant la tokenisation
2. **AST récursif** : Les pipes créent un arbre binaire naturellement récursif
3. **Builtins dans le parent** : Pour modifier l'état du shell

<img width="200" alt="image" src="https://github.com/user-attachments/assets/3cf91bdb-25e3-480f-9533-3ab7e1ee3588" />

---

## Compétences acquises

Ce projet nous a permis de développer :

- Compréhension approfondie des **processus Unix** (fork, exec, wait)
- Approfondir la maîtrise de la **gestion mémoire** en C
- Implémentation de **structures de données** complexes (AST, linked lists)
- Gestion des **signaux** et des **file descriptors**
- Travail en **binôme** avec répartition des tâches

<img width="300" alt="image" src="https://github.com/user-attachments/assets/2e35b6bf-c85d-41a6-9e63-23fecc3cf53c" />

---

<div align="center">
</div>




# Architecture global
<img alt="image" src="https://github.com/user-attachments/assets/7252cc6b-f63f-4c0c-81e1-dc6d11e9fd3a" />

# Exemple du flow d'une commande
<img alt="image" src="https://github.com/user-attachments/assets/93087866-8814-427b-909e-41a8546613b2" />



═══════════════════════════════════════════════════════════════════════
                         MINISHELL - FLUX D'EXÉCUTION
═══════════════════════════════════════════════════════════════════════

┌─────────────────────────────────────────────────────────────────────┐
│                            1. MAIN (main.c)                         │
└─────────────────────────────────────────────────────────────────────┘
                                   │
                                   ▼
                    ┌──────────────────────────┐
                    │   init_shell()           │
                    │   - Copie env            │
                    │   - exit_status = 0      │
                    │   - Setup signals        │
                    └──────────────────────────┘
                                   │
                                   ▼
                    ┌──────────────────────────┐
                    │   BOUCLE PRINCIPALE      │
                    │   while(1)               │
                    └──────────────────────────┘
                                   │
                                   ▼
┌─────────────────────────────────────────────────────────────────────┐
│                      2. LECTURE (readline)                          │
│   line = readline("El Cancer > ")                                   │
└─────────────────────────────────────────────────────────────────────┘
                                   │
                                   ▼
┌─────────────────────────────────────────────────────────────────────┐
│              3. PROCESS_INPUT (main_utils.c)                        │
└─────────────────────────────────────────────────────────────────────┘
                                   │
                    ┌──────────────┴──────────────┐
                    ▼                             ▼
        ┌───────────────────┐        ┌───────────────────────┐
        │   LEXER           │        │  handle_multiline     │
        │  (lexer.c)        │        │  (gère '\' en fin)    │
        └───────────────────┘        └───────────────────────┘
                    │
        ┌───────────┴───────────┐
        │   Tokenisation:       │
        │   - Quotes (' ")      │
        │   - Variables ($)     │
        │   - Pipes (|)         │
        │   - Redirections      │
        │     (>, >>, <, <<)    │
        └───────────┬───────────┘
                    │
                    ▼
        ┌─────────────────────────────────┐
        │   Token list:                   │
        │   echo → WORD                   │
        │   hello → WORD                  │
        │   > → TOKEN_REDIRECT_OUT        │
        │   file → WORD                   │
        │   | → TOKEN_PIPE                │
        │   cat → WORD                    │
        └─────────────────────────────────┘
                    │
                    ▼
┌─────────────────────────────────────────────────────────────────────┐
│              4. PARSER (parser.c)                                   │
│   ast = parse(tokens)                                               │
└─────────────────────────────────────────────────────────────────────┘
                    │
        ┌───────────┴───────────┐
        │   Trouve pipes?       │
        └───────────┬───────────┘
                    │
        ┌───────────┴───────────────────────┐
        │                                   │
        ▼ OUI                               ▼ NON
┌──────────────────┐              ┌──────────────────────┐
│  NODE_PIPE       │              │  NODE_COMMAND        │
│  Récursif:       │              │                      │
│  left = parse()  │              │  extract_redirections│
│  right = parse() │              │  args = tokens[]     │
└──────────────────┘              └──────────────────────┘
                    │
                    ▼
        ┌─────────────────────────────┐
        │   AST (Arbre):              │
        │                             │
        │        PIPE                 │
        │       /    \                │
        │   CMD1     CMD2             │
        │   echo     cat              │
        │   args[]   args[]           │
        │   >file    (stdin: pipe)    │
        └─────────────────────────────┘
                    │
                    ▼
┌─────────────────────────────────────────────────────────────────────┐
│          5. HEREDOC PROCESSING (heredoc_process.c)                  │
│   process_all_heredocs(ast, shell)                                  │
└─────────────────────────────────────────────────────────────────────┘
                    │
        ┌───────────┴───────────────────┐
        │   Pour chaque heredoc (<<):   │
        │   1. fork()                   │
        │   2. pipe()                   │
        │   3. Child lit stdin          │
        │   4. Écrit dans pipe          │
        │   5. Parent garde fd[0]       │
        └───────────────────────────────┘
                    │
                    ▼
┌─────────────────────────────────────────────────────────────────────┐
│          6. EXECUTE_COMMAND (main_utils.c)                          │
└─────────────────────────────────────────────────────────────────────┘
                    │
                    ▼
        ┌───────────────────────┐
        │   init_mother()       │
        │   - root = ast        │
        │   - envp = env[]      │
        │   - shell = &shell    │
        └───────────────────────┘
                    │
                    ▼
┌─────────────────────────────────────────────────────────────────────┐
│              7. MOTHER_EXEC (executor.c)                            │
└─────────────────────────────────────────────────────────────────────┘
                    │
        ┌───────────┴────────────┐
        │   Type de noeud?       │
        └───────────┬────────────┘
                    │
        ┌───────────┴───────────────────────┐
        │                                   │
        ▼ NODE_PIPE                         ▼ NODE_COMMAND
┌────────────────────────┐          ┌────────────────────────┐
│  PIPE_EXEC             │          │  CMD_EXEC              │
│  (pipeline.c)          │          │  (executor.c)          │
└────────────────────────┘          └────────────────────────┘
        │                                   │
        │                       ┌───────────┴────────────┐
        │                       │                        │
        │                       ▼                        ▼
        │              ┌─────────────────┐    ┌──────────────────┐
        │              │  is_builtin()?  │    │  Commande externe│
        │              └─────────────────┘    └──────────────────┘
        │                       │                      │
        │              ┌────────┴────────┐             │
        │              ▼                 ▼             │
        │      ┌─────────────┐  ┌──────────────┐       │
        │      │  BUILTIN    │  │  find_command│       │
        │      │  (direct)   │  │  fork+execve │       │
        │      └─────────────┘  └──────────────┘       │
        │                                              │
        ▼                                              ▼

═══════════════════════════════════════════════════════════════════════
                        DÉTAILS PAR TYPE DE COMMANDE
═══════════════════════════════════════════════════════════════════════

┌─────────────────────────────────────────────────────────────────────┐
│  A. COMMANDE SIMPLE: echo hello                                     │
└─────────────────────────────────────────────────────────────────────┘

readline("El Cancer > ")
    │
    ▼ "echo hello"
lexer()
    │
    ▼ [WORD:"echo"] → [WORD:"hello"]
parse()
    │
    ▼ NODE_COMMAND { args=["echo", "hello"] }
mother_exec()
    │
    ▼ cmd_exec()
    │
    ▼ is_builtin("echo") → OUI
    │
    ▼ builtin_echo(args)
    │   └─> ft_putstr_fd("hello", 1)
    ▼
exit_status = 0


┌─────────────────────────────────────────────────────────────────────┐
│  B. COMMANDE AVEC REDIRECTION: echo hello > file                    │
└─────────────────────────────────────────────────────────────────────┘

lexer()
    │
    ▼ [WORD:"echo"] → [WORD:"hello"] → [>] → [WORD:"file"]
parse()
    │
    ▼ extract_redirections()
    │   └─> outfile = "file", append = 0
    │
    ▼ NODE_COMMAND { args=["echo","hello"], outfile="file" }
cmd_exec()
    │
    ▼ exec_builtin_with_redir()
    │   1. Sauvegarde stdout (saved_out = dup(1))
    │   2. open("file", O_CREAT|O_TRUNC)
    │   3. dup2(fd, STDOUT_FILENO)
    │   4. builtin_echo()  ───> Écrit dans file
    │   5. dup2(saved_out, STDOUT_FILENO)
    │   6. close(saved_out)
    ▼
exit_status = 0


┌─────────────────────────────────────────────────────────────────────┐
│  C. PIPE: echo hello | cat                                          │
└─────────────────────────────────────────────────────────────────────┘

lexer()
    │
    ▼ [WORD:"echo"] → [WORD:"hello"] → [PIPE] → [WORD:"cat"]
parse()
    │
    ▼ find_pipe() → Trouvé!
    │
    ▼ NODE_PIPE
        │
        ├─> left: NODE_COMMAND { args=["echo","hello"] }
        │
        └─> right: NODE_COMMAND { args=["cat"] }
mother_exec()
    │
    ▼ pipe_exec()
        │
        ├─> pipe(fd)  [fd[0]=read, fd[1]=write]
        │
        ├─> fork() → pid1
        │   └─> Child 1:
        │       1. dup2(fd[1], STDOUT)  (sortie → pipe)
        │       2. close(fd[0], fd[1])
        │       3. mother_exec(left)
        │       4. builtin_echo() ────> Écrit dans pipe
        │       5. exit(0)
        │
        ├─> fork() → pid2
        │   └─> Child 2:
        │       1. dup2(fd[0], STDIN)   (entrée ← pipe)
        │       2. close(fd[0], fd[1])
        │       3. mother_exec(right)
        │       4. execve("/bin/cat")  ───> Lit depuis pipe
        │       5. exit(0)
        │
        ├─> Parent:
        │   1. close(fd[0], fd[1])
        │   2. waitpid(pid1)
        │   3. waitpid(pid2, &status)
        │   4. exit_status = WEXITSTATUS(status)
        │
        ▼
exit_status = 0


┌─────────────────────────────────────────────────────────────────────┐
│  D. HEREDOC: cat << EOF                                             │
└─────────────────────────────────────────────────────────────────────┘

lexer()
    │
    ▼ [WORD:"cat"] → [HEREDOC:"<<"] → [WORD:"EOF"]
parse()
    │
    ▼ NODE_COMMAND { args=["cat"], heredocs->delim="EOF" }
process_all_heredocs()
    │
    ▼ process_single_heredoc()
        │
        ├─> pipe(pipefd)
        │
        ├─> fork()
        │   └─> Child:
        │       1. Boucle readline("> ")
        │       2. Compare line == "EOF"
        │       3. write(pipefd[1], line)
        │       4. exit(0)
        │
        ├─> Parent:
        │   1. close(pipefd[1])
        │   2. waitpid()
        │   3. heredoc->fd = pipefd[0]  (garde fd ouvert)
        │
        ▼
cmd_exec()
    │
    ▼ fork() + execve("/bin/cat")
    │
    └─> setup_child_input()
        └─> dup2(heredoc->fd, STDIN)  ───> cat lit depuis heredoc
    ▼
exit_status = 0


┌─────────────────────────────────────────────────────────────────────┐
│  E. BUILTIN CD: cd /tmp                                             │
└─────────────────────────────────────────────────────────────────────┘

cmd_exec()
    │
    ▼ is_builtin("cd") → OUI
    │
    ▼ builtin_cd(args, &env)
    │   1. get_target_dir()
    │       └─> Si "~" → HOME
    │       └─> Si "-" → OLDPWD
    │       └─> Sinon → args[1]
    │   2. old_pwd = getcwd()
    │   3. chdir(target_dir)
    │   4. update_pwd_vars()
    │       └─> OLDPWD = old_pwd
    │       └─> PWD = getcwd()
    │
    ▼
exit_status = 0 (ou 1 si erreur)


┌─────────────────────────────────────────────────────────────────────┐
│  F. COMMANDE EXTERNE: /bin/ls -la                                   │
└─────────────────────────────────────────────────────────────────────┘

cmd_exec()
    │
    ▼ is_builtin("ls") → NON
    │
    ▼ find_command("ls", envp)
    │   1. check_absolute_path() → Non
    │   2. Cherche dans PATH
    │   3. Retourne "/bin/ls"
    │
    ▼ execute_external_command()
        │
        ├─> fork()
        │   └─> child_exec()
        │       1. setup_child_input()   (redirections <)
        │       2. setup_child_output()  (redirections >)
        │       3. execve("/bin/ls", ["-la"], envp)
        │
        ├─> Parent:
        │   1. waitpid(&status)
        │   2. exit_status = WEXITSTATUS(status)
        │
        ▼
exit_status = 0


═══════════════════════════════════════════════════════════════════════
                      GESTION DES SIGNAUX (signals.c)
═══════════════════════════════════════════════════════════════════════

┌─────────────────────────────────────────────────────────────────────┐
│  MODE INTERACTIF (readline)                                         │
│  setup_signals_interactive()                                        │
│                                                                     │
│  SIGINT (Ctrl+C):   → Affiche nouveau prompt                        │
│  SIGQUIT (Ctrl+\):  → Ignoré                                        │
└─────────────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────────────┐
│  MODE EXECUTION (fork/wait)                                         │
│  setup_signals_child()                                              │
│                                                                     │
│  SIGINT:   → Ignoré (parent), traité par child                      │
│  SIGQUIT:  → Ignoré (parent), traité par child                      │
└─────────────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────────────┐
│  MODE CHILD (execve)                                                │
│  setup_signals_exec()                                               │
│                                                                     │
│  SIGINT:   → SIG_DFL (termine le processus)                         │
│  SIGQUIT:  → SIG_DFL (core dump)                                    │
└─────────────────────────────────────────────────────────────────────┘


═══════════════════════════════════════════════════════════════════════
                        STRUCTURES DE DONNÉES
═══════════════════════════════════════════════════════════════════════

t_shell {
    t_env *env;              // Liste chaînée des variables
    int exit_status;         // Dernier code de retour
    t_ast *ast;              // Arbre syntaxique
    char **envp;             // Tableau env pour execve
}

t_mother {
    t_ast *root;             // AST complet
    char **envp;             // Environnement
    t_shell *shell;          // Pointeur vers shell
}

t_ast {
    t_node_type type;        // NODE_PIPE | NODE_COMMAND
    char **args;             // ["cmd", "arg1", "arg2"]
    char *infile;            // Fichier < input
    char *outfile;           // Fichier > output
    int append;              // 1 si >>, 0 si >
    t_heredoc *heredocs;     // Liste des <<
    t_ast *left;             // Sous-arbre gauche
    t_ast *right;            // Sous-arbre droit
}

t_token {
    t_token_type type;       // WORD | PIPE | REDIRECT_*
    char *value;             // Contenu du token
    t_token *next;           // Token suivant
}