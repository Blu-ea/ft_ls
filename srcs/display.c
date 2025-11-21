//
// Created by Blu-ea on 27/05/25.
//

#include "../incs/ft_ls.h"

static void display_items(char pathname[4096], const t_flags flags, t_list* items_to_print, t_list_padding padding)
{
	if (items_to_print == NULL)
		return;
	if (flags.list)
	{
		while (items_to_print)
		{
			display_item_stats(items_to_print->content, flags.list, padding, pathname);
			items_to_print = items_to_print->next;
			ft_putchar_fd('\n', 1);
		}
	}
	else // no list flags, so we print everything in columns
	{
		const size_t nb_of_item = ft_lstsize(items_to_print);
		size_t column_size[nb_of_item + 1];
		ft_bzero(column_size, sizeof(size_t) * (nb_of_item + 1));
		size_t nb_of_line = 1;
		const size_t nb_column = calc_column_size(items_to_print, nb_of_item, column_size, &nb_of_line);
		char *items_names[nb_of_item + 1];
		size_t i = 0;
		while (items_to_print)
		{
			items_names[i] = ((t_item*)items_to_print->content)->pathname;
			items_to_print = items_to_print->next;
			i++;
		}
		size_t current_line = 0;
		size_t current_column = 0;
		while (current_line < nb_of_line)
		{
			current_column = 0;
			while (current_column < nb_column)
			{
				const size_t index = current_line + current_column * nb_of_line;
				if (index < nb_of_item)
				{
					ft_putstr_fd(items_names[index], 1);
					if (column_size[current_column + 1] != 0)
						{
						const size_t padding_size = column_size[current_column] - ft_strlen(items_names[index]);
						for (size_t _ = 0; _ < padding_size; _++ )
							write(1, " ", 1);
					}
				}
				current_column++;
			}
			write(1, "\n", 1);
			current_line++;
		}
	}
}

static void display_folder_content(t_item* dir, t_flags flags, bool show_name_folder, bool first)
{
	t_list *item_to_print = get_items_from_folder(dir->pathname, flags.filter);
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

		padding = get_padding_list_flag(item_to_print, flags.list);
	}
	sort_items_merge(&item_to_print, flags.compare, flags.reverse);
	t_list *root_item = item_to_print;
	display_items(dir->pathname, flags, item_to_print, padding);

	if (flags.recursive) // and **ONLY** when all the items are print out, we open the folder recursivly;
	{
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

void display_ls(t_ls_lst_parms chain_items, const t_flags flags)
{
	bool first = true;
	size_t show_name_folder = false;
	if (chain_items.files)
	{
		t_list *files = chain_items.files;
		sort_items_merge(&files, flags.compare, flags.reverse);

		char path[PATH_MAX] = {0};
		path[0] = '.';
		display_items(path, flags, files, get_padding_list_flag(files, flags.list));
		show_name_folder = true;
		first = false;
	}
	if (chain_items.dirs)
	{
		t_list* dir = chain_items.dirs;
		show_name_folder = !!(ft_lstsize(dir) - 1) || show_name_folder;
		sort_items_merge(&dir, flags.compare, flags.reverse);
		while(dir)
		{
			display_folder_content(dir->content, flags, show_name_folder, first);
			first = false;
			dir = dir->next;
		}
	}
}
