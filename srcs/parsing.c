//
// Created by Blu-ea on 20/05/25.
//

#include <stdbool.h>
#include "../incs/ft_ls.h"

static void init_flags(t_flags* flag)
{
	flag->recursive = false;
	flag->list = false;
	flag->all = false;
	flag->reverse = false;
	flag->time = false;
	flag->_flag_error = false;
	flag->paths = ft_calloc(2, sizeof(char*));
	if (flag->paths == NULL)
		return;
	flag->paths[0] = ft_strdup(".");
}

void check_flags(t_flags *flags, const char* i_flags)
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
				flags->all = true;
				break;
			case 'r':
				flags->reverse = true;
				break;
			case 't':
				flags->time = true;
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

void add_path(t_flags* flags, const char *new_path)
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
	(void)argv;
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


void print_flags(t_flags pars) // todo: remove before final push
{
	printf("\033[91m == Flags: ==\n");
	printf("rec : %s\n", pars.recursive ? "true" : "false");
	printf("list : %s\n", pars.list ? "true" : "false");
	printf("all : %s\n", pars.all ? "true" : "false");
	printf("revr : %s\n", pars.reverse ? "true" : "false");
	printf("time : %s\n", pars.time ? "true" : "false");
	if (pars.paths)
		for(int i = 0; pars.paths[i] != NULL; i++ )
			printf("path [%d]  : %s\n", i, pars.paths[i]);
	else
		printf("path : NULL\n");
	printf(" ===== \033[0m\n");
}