#ifndef FT_LS_H
# define FT_LS_H

#include <stdio.h> // printf

# include <dirent.h>
# include <pwd.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/stat.h>
# include <sys/types.h>

# include "dir_content.h"
#include <libft.h>

# define USAGE \
	"Usage: ./ft_ls [OPTION] [FOLDER]\n"\
	"Options:\n"\
	"	-R	Recursively list subdirectories encountered. When a symbolic link to a directory is encountered, the directory shall not be recursively listed.\n"\
	"	-a	Write out all directory entries, including those whose names begin with a <period> ('.').\n"\
	"	-l	Write out in long format.\n"\
	"	-r	Reverse the order of the sort to get reverse collating sequence oldest first, or smallest file size first depending on the other options given.\n"\
	"	-t	Sort with the primary key being time modified (most recently modified first) and the secondary key being filename in the collating sequence.  For a symbolic link, the time used as the sort key is that of the symbolic link itself.\n"

void get_file(char *filename);

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
void print_flags(t_flags pars); // todo: Remove before final push

#endif //FT_LS_H
