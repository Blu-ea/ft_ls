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

void get_file(char *filename);


#endif //FT_LS_H
