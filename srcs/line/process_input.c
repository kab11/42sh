/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsathiad <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/20 02:04:52 by rsathiad          #+#    #+#             */
/*   Updated: 2019/03/22 20:39:02 by rsathiad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/line.h"

t_ctrl	g_kctrl[] =
{
	{K_UP, history_up},
	{K_DOWN, history_down},
	{K_LEFT, arrow_left},
	{K_RIGHT, arrow_right},
	{K_BS, arrow_backspace},
	{HOME_B, move_cursor_to_prompt},
	{END_B, move_cursor_to_end},
	{CTRL_UPK, move_killzone_up},
	{CTRL_DNK, move_killzone_down},
	{CTRL_LFK, move_cursor_to_previous_word},
	{CTRL_RTK, move_cursor_to_next_word},
};

t_ctrl	g_actrl[] =
{
	{CTRL_E, move_cursor_to_next_word},
	{CTRL_W, move_cursor_to_previous_word},
	{CTRL_K, cut_to_prompt},
	{CTRL_L, cut_to_endline},	//BUG: relundant char
	{CTRL_G, paste_clipboard},
	{CTRL_U, copy_next_word},
	{CTRL_D, send_eof},
	{K_TAB, auto_complete},
	{1, copy_line},
};

void	process_input(long num, int len, t_edit *edit)
{
	int		i;

	i = -1;
	if (len == 1)
	{
		if (num <= 31)
		{
			while (++i < ACTRL_NUM)
				if (num == g_actrl[i].value)
					g_actrl[i].f(edit);
		}
		else if (num != 127 && num != '\n')
			add_to_killzone(num, edit);
		else if (num != '\n')
			arrow_backspace(edit);
	}
	else if (len == 3 || len == 6)
	{
		while (++i < KCTRL_NUM)
			if (num == g_kctrl[i].value)
				g_kctrl[i].f(edit);
	}
	else if (len == 4)
		delete_from_killzone(edit);
}

void	add_to_killzone(int num, t_edit *edit)
{
	size_t			cursor_to_null;
	char			*s;
	unsigned int	buf;

	s = edit->array[edit->killzone]->line;
	buf = edit->array[edit->killzone]->buffpos;
	cursor_to_null = ft_strlen(s + buf);
	ft_memmove(s + buf + 1, s + buf, cursor_to_null);
	tputs(tgetstr("nd", NULL), 0, term_putc);
	s[buf] = num;
	edit->array[edit->killzone]->buffpos++;
	edit->array[edit->killzone]->printlen++;
	edit->cur_col++;
}

void	delete_from_killzone(t_edit *edit)
{
	size_t			cursor_to_null;
	char			*s;
	unsigned int	buf;

	cursor_to_null = 0;
	s = edit->array[edit->killzone]->line;
	buf = edit->array[edit->killzone]->buffpos;
	if (buf == edit->array[edit->killzone]->printlen)
		return ;
	cursor_to_null = ft_strlen(s + buf);
	ft_memmove(s + buf, s + buf + 1, cursor_to_null);
	edit->array[edit->killzone]->printlen--;
	edit->cur_col--;
	if (buf == 0)
		edit->cur_col = 0;
}

void	save_cursor_pos(void)
{
	ft_putstr(tgetstr("sc", NULL));
}

void	recall_last_cursor_pos(void)
{
	ft_putstr(tgetstr("rc", NULL));
}
