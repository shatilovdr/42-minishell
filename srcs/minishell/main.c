/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnikifor <dnikifor@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 11:53:09 by dnikifor          #+#    #+#             */
/*   Updated: 2024/04/09 12:41:55 by dnikifor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

int	main(void)
{
	t_minishell	*ms;

	initialize_minishell(&ms);
	run_minishell(ms);
	rl_clear_history();
	ft_free_minishell(ms);
	return (0);
}
