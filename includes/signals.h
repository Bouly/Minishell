/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abendrih <abendrih@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 10:00:22 by abendrih          #+#    #+#             */
/*   Updated: 2025/11/21 21:20:32 by abendrih         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <termios.h>
# include <unistd.h>

/* Signal handlers */
void	handle_sigint(int sig);
void	handle_sigint_child(int sig);

/* Signal setup */
void	setup_signals_interactive(void);
void	setup_signals_child(void);
void	setup_signals_exec(void);
void	enable_ctrl_chars_display(void);

#endif
