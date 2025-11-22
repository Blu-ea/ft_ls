#ifndef DIR_CONTENT_H
# define DIR_CONTENT_H

# include "../libft/includes/libft.h"

typedef struct s_ls_lst_parms
{
	t_list* files;
	t_list* dirs;

}	t_ls_lst_parms;

typedef struct s_item_list
{
	char pathname [PATH_MAX];
	struct stat item_stat;

}	t_item;


typedef struct s_list_padding
{
	int link;
	int user;
	int group;
	int size;
}	t_list_padding;

#endif //DIR_CONTENT_H