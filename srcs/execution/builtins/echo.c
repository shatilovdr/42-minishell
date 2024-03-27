/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnikifor <dnikifor@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/02 15:26:40 by dnikifor          #+#    #+#             */
/*   Updated: 2024/03/27 11:18:44 by dnikifor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../headers/minishell.h"

/**
* @brief	A function that writes to the stdout
* @param	arr array of arguments or options if allowed
* @param	flag new line flag
* @param	i index
* @return	@c `SUCCESS`
*/
static int	echo_write(char **arr, int flag, int i)
{
	if (!flag)
	{
		while (arr[i])
		{
			ft_putstr_fd(arr[i], STDOUT_FILENO);
			if (arr[i + 1])
				ft_putchar_fd(SPACE, STDOUT_FILENO);
			i++;
		}
		ft_putchar_fd(NL, STDOUT_FILENO);
	}
	else
	{
		while (arr[i])
		{
			ft_putstr_fd(arr[i], STDOUT_FILENO);
			if (arr[i + 1])
				ft_putchar_fd(SPACE, STDOUT_FILENO);
			i++;
		}
	}
	return (SUCCESS);
}

/**
* @brief	A function that runs echo built-in command
* @param	arr array of arguments or options if allowed
* @param	i index
* @param	j index
* @param	nl_flag new line flag
* @return	@c `SUCCESS`
*/
int	run_echo(char **arr, int i, int j, int nl_flag)
{
	int	k;

	if (!arr[i])
		ft_putchar_fd(NL, STDOUT_FILENO);
	while (arr[i])
	{
		k = i;
		j = 0;
		if (arr[i][j] == DASH)
		{
			j++;
			while (arr[i][j] != NULL_TERM)
			{
				skip_characters(arr[i], &j, 110);
				if (arr[i][j] != NULL_TERM)
					return (echo_write(arr + k, nl_flag, 0));
			}
			i++;
			nl_flag = 1;
			continue ;
		}
		else
			return (echo_write(arr + k, nl_flag, 0));
	}
	return (SUCCESS);
}
