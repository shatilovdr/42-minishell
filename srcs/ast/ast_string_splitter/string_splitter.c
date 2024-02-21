/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_splitter.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnikifor <dnikifor@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 14:39:12 by dnikifor          #+#    #+#             */
/*   Updated: 2024/02/21 22:07:40 by dnikifor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../headers/minishell.h"

int	parenthesis_quotes_checker(t_node_data **node, char *str, int type,
	int i)
{
	int	status;

	while (i >= 0)
	{
		if (type == T_AND || type == T_OR)
			status = and_if_condition_block(node, str, type, i);
		else if (type == T_PIPE)
			status = pipe_block(node, str, type, i);
		else if (type == T_BRACKET)
			status = brackets_block(node, str, type);
		else if (type == T_COMMAND_BR)
			status = command_block(node, str, type);
		else if (type == T_COMMAND)
			status = command_without_bracket_block(node, str, type);
		else if (type == T_CMD_SIMPLE)
			status = set_node_cmd_simple(node, str, T_CMD_SIMPLE);
		if (status > 0)
			return (status);
		else if (status < 0)
			return (ft_error("Error\n", -1));
		i--;
	}
	return (parenthesis_quotes_checker(node, str, type + 1,
			ft_strlen(str) - 1));
}

// int	main()
// {
// 	t_node_data	*node;

// 	char str[1000] = "((111 | 222) > out || 333) ";
// 	int status = parenthesis_quotes_checker(&node, str, T_AND,
// 			ft_strlen(str) - 1);
	
// 	printf("status: %d\n", status);
// 	printf("type: %d\n", node->type);
// 	printf("$%s$\n", node->str_left);
// 	printf("$%s$\n", node->str_right);
// 	return (0);
// }
