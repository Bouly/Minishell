/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abendrih <abendrih@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 10:00:22 by abendrih          #+#    #+#             */
/*   Updated: 2025/11/19 15:52:00 by abendrih         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

# include <signal.h>
# include <stdio.h>
# include <readline/readline.h>
# include <unistd.h>
# include <termios.h>

/* Signal handlers */
void	handle_sigint(int sig);

/* Signal setup */
void	setup_signals_interactive(void);
void	setup_signals_exec(void);
void	disable_ctrl_chars_display(void);

#endif
