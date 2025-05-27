#ifndef DIR_CONTENT_H
# define DIR_CONTENT_H

# include <stdbool.h>

#include "libft.h"

typedef struct s_ls_lst_parms
{
	t_list* files;
	t_list* dirs;

}	t_ls_lst_parms;

typedef struct s_item_list
{
	char* pathname;
	struct stat item_stat;

}	t_item_list;


#endif //DIR_CONTENT_H