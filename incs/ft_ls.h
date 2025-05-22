#ifndef FT_LS_H
# define FT_LS_H

#include <stdio.h> // printf

# include <sys/stat.h>
# include <sys/types.h>
# include <pwd.h>
# include <unistd.h>
# include <dirent.h>
# include <stdlib.h>

# include "dir_content.h"
#include "../libft/includes/libft.h"

void get_file(char *filename);

typedef struct s_flags
{
	bool recursive;
	bool list;
	bool all;
	bool reverse;
	bool time;
	char **paths;

}	t_flags;

t_flags	parsing(int, char**);
void print_flags(t_flags pars); // todo: Remove before final push

#endif //FT_LS_H
