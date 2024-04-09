/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_executable.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnikifor <dnikifor@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 14:02:40 by dshatilo          #+#    #+#             */
/*   Updated: 2024/04/09 13:58:18 by dnikifor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

static int	check_path_provided(char *cmd_name, char **cmd_in_path);
static int	find_cmd_in_path(char *cmd_name, char **paths, char **cmd_in_path);
static char	*cmd_to_search(char *cmd_name, int *length, char **paths);
static int	check_cmd_in_path(char **candidate, int len,
				char *cmd_name, char *path);

/**
 * @brief	Find executable in specified paths
 * @note	Function searches for an executable command in the specified
 * paths. If the command includes a path, it directly checks whether the path
 * is valid. Otherwise, it searches the paths provided to find the executable.
 * If found, the command is updated with the full path of the executable
 * @param	command The command to find, possibly with a path
 * @param	paths An array of paths to search for the command
 * @return	Returns @c `0` if the executable is found and updated successfully,
 * @c `CMD_NF_FAILURE` if the command is not found,
 * @c `CMD_PD_FAILURE` if permission is denied,
 * @c `ISDIR_FAILURE` if the command is a directory
 */
int	find_executable(char **command, char **paths)
{
	int		status;
	char	*cmd_in_path;

	cmd_in_path = NULL;
	if (ft_strchr(command[0], '/') == NULL)
		status = find_cmd_in_path(command[0], paths, &cmd_in_path);
	else
		status = check_path_provided(command[0], &cmd_in_path);
	if (status == 0)
	{
		if (ft_strchr(command[0], '/') == NULL)
			free(command[0]);
		command[0] = cmd_in_path;
	}
	else if (status == CMD_NF_FAILURE)
		print_err_msg(command[0], ": command not found\n");
	else if (status == CMD_PD_FAILURE)
		print_err_msg(command[0], ": Permission denied\n");
	else if (status == ISDIR_FAILURE)
	{
		print_err_msg(command[0], ": is a directory\n");
		status = CMD_PD_FAILURE;
	}
	return (status);
}

/**
 * @brief	Check if the provided path to the command is valid
 * @note	Function checks if the provided path to the command
 * exists and is executable.
 * If the path is valid, it updates the pointer to the command in path.
 * @param	cmd_name The provided path to the command
 * @param	cmd_in_path Pointer to store the valid path to the command
 * @return	Returns @c `0` if the path is valid and executable,
 * @c `CMD_NF_FAILURE` if the command is not found,
 * @c `ISDIR_FAILURE` if the command is a directory,
 * @c `CMD_PD_FAILURE` if permission is denied
 */
static int	check_path_provided(char *cmd_name, char **cmd_in_path)
{
	struct stat	st;

	if (access(cmd_name, F_OK) != 0)
		return (CMD_NF_FAILURE);
	stat(cmd_name, &st);
	if (S_ISDIR(st.st_mode))
		return (ISDIR_FAILURE);
	if (access(cmd_name, X_OK) != 0)
		return (CMD_PD_FAILURE);
	*cmd_in_path = cmd_name;
	return (0);
}

/**
 * @brief	Find command in the specified paths
 * @note	This function searches for a command in the specified paths.
 * It constructs a full path by combining each path with the command name,
 * then checks if the constructed path is valid. If found, the valid path
 * to the command is stored in @c `cmd_in_path`
 * @param	cmd_name The name of the command to find
 * @param	paths An array of paths to search for the command
 * @param	cmd_in_path Pointer to store the valid path to the command
 * @return	Returns @c `0` if the command is found and its path is updated,
 * @c `MALLOC_ERR` if memory allocation fails,
 * @c `CMD_NF_FAILURE` if the command is not found,
 * @c `CMD_PD_FAILURE` if permission is denied
 */
static int	find_cmd_in_path(char *cmd_name, char **paths, char **cmd_in_path)
{
	int		status;
	char	*candidate;
	int		len;
	int		i;

	len = 0;
	candidate = cmd_to_search(cmd_name, &len, paths);
	if (!candidate)
		return (MALLOC_ERR);
	i = 0;
	while (paths[i])
	{
		status = check_cmd_in_path(&candidate, len, cmd_name, paths[i]);
		if (status != 1)
		{
			if (status == 0)
				*cmd_in_path = candidate;
			else if (status == CMD_PD_FAILURE)
				free(candidate);
			return (status);
		}
		i++;
	}
	free(candidate);
	return (CMD_NF_FAILURE);
}

/**
 * @brief	Construct command path for searching
 * @note	This function constructs a command path by combining each path
 * in the paths array with the command name. The length of the longest path
 * is determined to allocate sufficient memory for the constructed path.
 * The constructed path is returned to be used for searching.
 * @param	cmd_name The name of the command
 * @param	length Pointer to store the length of the constructed path
 * @param	paths An array of paths to search for the command
 * @return	Returns a dynamically allocated string containing the constructed
 * path, or @c `NULL` if memory allocation fails.
 */
static char	*cmd_to_search(char *cmd_name, int *length, char **paths)
{
	char	*cmd;
	int		len;
	int		curr_len;
	int		i;

	i = 0;
	len = 0;
	while (paths[i])
	{
		curr_len = ft_strlen(paths[i]);
		if (curr_len > len)
			len = curr_len;
		i++;
	}
	len += ft_strlen(cmd_name) + 1 + 1;
	cmd = (char *)ft_calloc(len, sizeof(char));
	if (!cmd)
	{
		print_err_msg(cmd_name, ": allocation error occured\n");
		return (NULL);
	}
	*length = len;
	return (cmd);
}

/**
 * @brief	Check if the command exists and is executable in the given path.
 * @note	This function checks if the command exists and is executable
 * in the specified path. It constructs the full path to the command by
 * concatenating the path with the command name and then checks if the
 * constructed path is valid.
 *
 * @param	candidate Pointer to the constructed command path
 * @param	len Length of the constructed command path
 * @param	cmd_name The name of the command
 * @param	path The path to search for the command
 * @return	Returns @c `0` if the command exists and is executable,
 * @c `ISDIR_FAILURE` if the command is a directory,
 * @c `CMD_PD_FAILURE` if permission is denied,
 * @c `1` if the command does not exist
 */
static int	check_cmd_in_path(char **candidate, int len,
	char *cmd_name, char *path)
{
	char		*temp;
	struct stat	st;

	temp = *candidate;
	ft_memset(temp, 0, len);
	ft_strlcat(temp, path, len);
	ft_strlcat(temp, "/", len);
	ft_strlcat(temp, cmd_name, len);
	if (access(temp, F_OK) == 0)
	{
		stat(cmd_name, &st);
		if (S_ISDIR(st.st_mode))
			return (ISDIR_FAILURE);
		if (access(temp, X_OK) == 0)
			return (0);
		else
			return (CMD_PD_FAILURE);
	}
	else
		return (1);
}
