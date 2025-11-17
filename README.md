Brouillon

# Minishell - aka MiniMerde

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

---

## Utilisation

### Installation

```bash
git clone caca.git
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

---

### Points clés de l'architecture

1. **Lexer avec expansion** : Les variables sont expand pendant la tokenisation
2. **AST récursif** : Les pipes créent un arbre binaire naturellement récursif
3. **Builtins dans le parent** : Pour modifier l'état du shell

---

## Compétences acquises

Ce projet nous a permis de développer :

- Compréhension approfondie des **processus Unix** (fork, exec, wait)
- Approfondir la maîtrise de la **gestion mémoire** en C
- Implémentation de **structures de données** complexes (AST, linked lists)
- Gestion des **signaux** et des **file descriptors**
- Travail en **binôme** avec répartition des tâches

---

<div align="center">
</div>




# Architecture global
<img alt="image" src="https://github.com/user-attachments/assets/7252cc6b-f63f-4c0c-81e1-dc6d11e9fd3a" />

# Exemple du flow d'une commande
<img alt="image" src="https://github.com/user-attachments/assets/93087866-8814-427b-909e-41a8546613b2" />

