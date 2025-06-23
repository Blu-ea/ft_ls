#include "../incs/ft_ls.h"

void print_file(t_item *temp_item)
{
	t_item *item = temp_item;
	printf("--- \n");
	printf("- path - %s\n", item->pathname);
	printf("- grid - %u\n", item->item_stat.st_gid);
	printf("--- \n");
}

int main(int argc, char **argv){
	t_flags flags = parsing(argc, argv);
	// print_flags(flags);
	if (flags.paths == NULL || flags._flag_error)
		return 1; // An error append.

	t_ls_lst_parms lst_parms = get_parms(flags.paths);

	display_ls(lst_parms, flags);
	ft_free_2d_array(flags.paths);
	ft_lstclear(&lst_parms.files, free);
	ft_lstclear(&lst_parms.dirs, free);
}
