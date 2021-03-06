/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csinglet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/18 15:09:17 by csinglet          #+#    #+#             */
/*   Updated: 2019/02/18 15:09:17 by csinglet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/42sh.h"

int		main(void)
{
	char		*line;
	t_token		*token;
	t_astnode	*astree;
	t_sh		sh;

	ft_bzero((void *)&sh, sizeof(t_sh));
	env_init(&sh);
	command_init(&sh);
	g_power = 1;
	while (g_power)
	{
		signals_init();

		if ((line = ft_readline(&sh)) == NULL)			
			exit(0);
		printf("\n");
		
		token = tokenize(line);

		build_astree(token, &astree);

		// printBinaryTree(astree);

		execute_astree(astree, &sh);	//TODO: Change env built-in

		free(line);
		free_token_list(token);
		free_astree(&astree);
	}
}
