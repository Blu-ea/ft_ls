#ifndef DIR_CONTENT_H
# define DIR_CONTENT_H

# include <stdbool.h>

#include "../libft/includes/libft.h"

typedef struct s_ls_lst_parms
{
	t_list* files;
	size_t	max_dg_lenght; // used in -l, to do the spacing for the size
	t_list* dirs;

}	t_ls_lst_parms;

typedef struct s_item_list
{
	char* pathname;
	struct stat item_stat;

}	t_item;


#endif //DIR_CONTENT_H