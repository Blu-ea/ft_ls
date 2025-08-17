//
// Created by Blu-ea on 27/05/25.
//

#include "../incs/ft_ls.h"

void display_folder_content(t_item* dir, t_flags flags, bool show_name_folder, bool first)
{
	t_list *item_to_print = get_items_from_folder(dir->pathname, flags.all);
	if (item_to_print == (void*) -1)
		return;
	if (show_name_folder || flags.recursive)
	{
		if (!first)
			ft_putchar_fd('\n', 1);
		ft_printf("%s:\n", dir->pathname);
	}
	t_list_padding padding = {0,0,0,0};
	if (flags.list)
	{
		ft_putstr_fd("total ", 1);
		t_list *root_item = item_to_print;
		long int total = 0;
		while (item_to_print)
		{
			t_item *item = item_to_print->content;
			total += item->item_stat.st_blocks;  // By man 7 inode : the size of a block is not defined by POSIX.1 so it may be 1024.
			item_to_print = item_to_print->next;
		}
		ft_putnbr_fd(total / 2, 1); // i don't know why but ls seems to print for 1k blocks, tho POSIX define them as size of 512.
		ft_putchar_fd('\n', 1);
		item_to_print = root_item;

		padding = get_padding(item_to_print, flags.list);
	}
	sort_items(&item_to_print, flags.time, flags.reverse);
	t_list *root_item = item_to_print;
	while (item_to_print) // first we print the items
	{
		display_item_stats(item_to_print->content, flags.list, padding, dir->pathname);
		if (!flags.list && item_to_print->next)
			write(1, "  ", 2);
		item_to_print = item_to_print->next;
		if (flags.list)
			ft_putchar_fd('\n', 1);
	}
	if (!flags.list)
		ft_putchar_fd('\n', 1);
	if (flags.recursive) // and **ONLY** when all the items are print out, we open the folder recursivly;
	{
		item_to_print = root_item;
		while (item_to_print) // check for the item, too see if there is a folder
		{
			t_item *recursive_item = item_to_print->content;

			if (S_ISDIR(recursive_item->item_stat.st_mode) && ft_memcmp(recursive_item->pathname, ".", 2) != 0
			&& ft_memcmp(recursive_item->pathname, "..", 3) != 0)
			{
				bool is_there_slash = 0;
				if (dir->pathname[ft_strlen(dir->pathname) - 1] != '/')
					is_there_slash = 1;
				ft_memmove(recursive_item->pathname + ft_strlen(dir->pathname) + is_there_slash, recursive_item->pathname, ft_strlen(recursive_item->pathname));
				ft_memcpy(recursive_item->pathname, dir->pathname, ft_strlen(dir->pathname));
				if (is_there_slash)
					recursive_item->pathname[ft_strlen(dir->pathname)] = '/';
				display_folder_content(recursive_item, flags, true, false);
			}
			item_to_print = item_to_print->next;
		}
	}
	ft_lstclear(&root_item, free);
}

void display_ls(t_ls_lst_parms chain_items, t_flags flags)
{
	bool first = true;
	size_t show_name_folder = false;
	if (chain_items.files)
	{
		t_list *files = chain_items.files;
		sort_items(&files, flags.time, flags.reverse);

		char path[PATH_MAX] = {};
		path[0] = '.';
		while(files)
		{
			// t_list_padding padding = get_padding(files, flags.list);
			display_item_stats(files->content, flags.list, get_padding(files, flags.list), path);
			if ( files->next)
				ft_putchar_fd('\n', 1);
			files = files->next;
		}
		show_name_folder = true;
		first = false;
		ft_putchar_fd('\n', 1);
	}
	if (chain_items.dirs)
	{
		t_list* dir = chain_items.dirs;
		show_name_folder = !!(ft_lstsize(dir) - 1) || show_name_folder;
		sort_items(&dir, flags.time, flags.reverse);
		while(dir)
		{
			display_folder_content(dir->content, flags, show_name_folder, first);
			first = false;
			dir = dir->next;
		}
	}
}
