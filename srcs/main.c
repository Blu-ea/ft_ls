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

	t_ls_lst_parms lst_parms = get_parms(flags.paths, flags.recursive, flags.all);

	sort_items(&lst_parms.files, flags.time);
	// sort_items(lst_parms.dirs, flags.time);

	display_ls(lst_parms, flags);
	// ft_lstiter(lst_parms.dirs, (void (*)(void *))print_file);
}
