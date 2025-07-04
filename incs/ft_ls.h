#ifndef FT_LS_H
# define FT_LS_H

#include <stdio.h> // printf

# include <pwd.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <dirent.h>

# include "dir_content.h"
#include "../libft/includes/libft.h"

# define USAGE \
	"Usage: ./ft_ls [OPTION] [FOLDER]\n"\
	"Options:\n"\
	"	-R	Recursively list subdirectories encountered. When a symbolic link to a directory is encountered, the directory shall not be recursively listed.\n"\
	"	-a	Write out all directory entries, including those whose names begin with a <period> ('.').\n"\
	"	-l	Write out in long format.\n"\
	"	-r	Reverse the order of the sort to get reverse collating sequence oldest first, or smallest file size first depending on the other options given.\n"\
	"	-t	Sort with the primary key being time modified (most recently modified first) and the secondary key being filename in the collating sequence.\n"\
	"		For a symbolic link, the time used as the sort key is that of the symbolic link itself.\n"\
	"	--help	Display this help and exit.\n"

typedef struct s_flags
{
	bool recursive;
	bool list;
	bool all;
	bool reverse;
	bool time;
	bool _flag_error;
	char **paths;

}	t_flags;

t_flags	parsing(int, char**);
void	print_flags(t_flags pars); // todo: Remove before final push

t_ls_lst_parms get_parms(char **path);

t_list*	get_file(char **path, bool recursive);

void display_ls(t_ls_lst_parms lst_parms, t_flags flags);

t_list *get_items_from_folder(char *pathname, bool flag_all);

void sort_items(t_list** lst, bool flag_time, bool flag_reverse);

void recursive_get(t_list** parm_dir ,char *folder_path, bool all_flag);

#endif //FT_LS_H
