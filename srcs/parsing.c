//
// Created by Blu-ea on 20/05/25.
//

#include <stdbool.h>
#include "../incs/ft_ls.h"

static void init_flags(t_flags* flag)
{
	flag->recursive = false;
	flag->list = false;
	flag->filter = NO_HIDDEN;
	flag->reverse = false;
	flag->compare = compare_name;
	flag->_flag_error = false;
	flag->paths = malloc(sizeof(char*) * 2);
	if (flag->paths == NULL)
		return;
	flag->paths[0] = ft_strdup(".");
	flag->paths[1] = NULL;
}

static void check_flags(t_flags *flags, const char* i_flags)
{
	if (!ft_memcmp("--help", i_flags, 7))
	{
		ft_free_2d_array(flags->paths);
		flags->paths = NULL; // destroy the path to exit;
		ft_printf(USAGE);
		return;
	}
	size_t i = 1;
	const size_t len = ft_strlen(i_flags);
	while (i < len)
	{
		switch (i_flags[i])
		{
			case 'R':
				flags->recursive = true;
				break;
			case 'l':
				flags->list = true;
				break;
			case 'a':
				flags->filter = ALL;
				break;
			case 'A':
				flags->filter = ALMOST;
				break;
			case 'r':
				flags->reverse = true;
				break;
			case 't':
				flags->compare = compare_time;
				break;
			case 'U':
				flags->compare = no_sort;
				break;
			default:
				ft_putstr_fd("\033[31mft_ls: invalid option -- '", 2);
				ft_putchar_fd(i_flags[i], 2);
				ft_putstr_fd("'\033[0m\n", 2);
				flags->_flag_error = true;
				return;
		}
		i++;
	}
}

static void add_path(t_flags* flags, const char *new_path)
{
	static bool first = true;

	if (first)
	{
		first = false;
		free (flags->paths[0]);
		flags->paths[0] = ft_strdup(new_path);
		if (flags->paths[0] == NULL)
		{
			free(flags->paths);
			flags->paths = NULL;
		}
		return;
	}

	int tab_len = ft_tablen(flags->paths);
	char** new_path_list = ft_calloc(tab_len + 2, sizeof(char*));
	if (new_path_list == NULL)
	{
		ft_free_2d_array(flags->paths);
		flags->paths = NULL;
		return;
	}
	ft_memcpy(new_path_list, flags->paths, (tab_len) * sizeof(char*));
	new_path_list[tab_len] = ft_strdup(new_path);
	if (new_path_list[tab_len] == NULL)
	{
		ft_free_2d_array(flags->paths);
		flags->paths = NULL;
	}
	else
	{
		free(flags->paths);
		flags->paths = new_path_list;
	}
}

t_flags parsing(int argc, char **argv)
{
	t_flags flags;
	init_flags(&flags);
	if (flags.paths == NULL || flags.paths[0] == NULL)
		return flags;

	int i = 1;
	while (i < argc && flags.paths && !flags._flag_error )
	{
		if(argv[i][0] == '-')
			check_flags(&flags, argv[i]);
		else
			add_path(&flags, argv[i]);
		i++;
	}

	return flags;
}
