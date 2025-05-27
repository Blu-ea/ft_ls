#include "../incs/ft_ls.h"

void print_file(void *temp_item)
{
	t_item_list *item = temp_item;
	printf("--- \n");
	printf("- path - %s\n", item->pathname);
	printf("- grid - %u\n", item->item_stat.st_gid);
	printf("--- \n");
}

int main(int argc, char **argv){
	t_flags flags = parsing(argc, argv);
	print_flags(flags);
	if (flags.paths == NULL || flags._flag_error)
		return 1; // An error append.

	t_ls_lst_parms lst_parms = get_parms(flags.paths, flags.recursive, flags.all);

	// t_list* chain_items = get_file(flags.paths, flags.recursive);
	// printf("path in main -= %p\n", ((t_item_list *)files->content)->pathname);
	ft_lstiter(lst_parms.dirs, print_file);
	// (void) chain_items;
}
