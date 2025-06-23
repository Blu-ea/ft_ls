//
// Created by Blu-ea on 27/05/25.
//

#include "../incs/ft_ls.h"
#include <grp.h>
#include <time.h>

void stat_display(unsigned int mode_mask, __mode_t mode_value, char print_flag)
{
	if( mode_mask & mode_value)
		write(1, &print_flag,1);
	else
		write(1, "-", 1);
}


void stat_type(__mode_t mode_value)
{
	if (S_ISDIR(mode_value))
		write(1, "d",1); /* Directory */
	else if (S_ISBLK(mode_value))
		write(1, "b",1); /* Block special file */
	else if (S_ISCHR(mode_value))
		write(1, "c",1); /* Character special file */
	else if (S_ISLNK(mode_value))
		write(1, "l",1); /* Symbolic link */
	else if (S_ISFIFO(mode_value))
		write(1, "f",1); /* FIFO */
	else
		write(1, "-",1);
}

void stats_display(__mode_t mode_value)
{
	stat_type(mode_value);
	stat_display(S_IRUSR, mode_value, 'r');
	stat_display(S_IWUSR, mode_value, 'w');
	if (S_ISUID & mode_value)
	{
		if (!(S_IXUSR & mode_value)) write(1, "S",1);
		else if (S_IXUSR & mode_value) write(1, "s",1);
	}
	else
		stat_display(S_IXUSR, mode_value, 'x');

	stat_display(S_IRGRP, mode_value, 'r');
	stat_display(S_IWGRP, mode_value, 'w');
	if (S_ISGID & mode_value)
	{
		if (!(S_IXGRP & mode_value)) write(1, "S",1);
		else if (S_IXGRP & mode_value) write(1, "s",1);
	}
	else
		stat_display(S_IXGRP, mode_value, 'x');

	stat_display(S_IROTH, mode_value, 'r');
	stat_display(S_IWOTH, mode_value, 'w');
	if (S_ISVTX & mode_value && S_ISDIR(mode_value))
	{
		if (!(S_IXOTH & mode_value)) write(1, "T",1);
		else if (S_IXOTH & mode_value) write(1, "t",1);
	}
	else
		stat_display(S_IXOTH, mode_value, 'x');
}

void time_display(struct timespec ts)
{
	const time_t *last_update = &ts.tv_sec;
	const time_t now = time(NULL);

	char *time_str = ctime(last_update);
	write(1, time_str + 4, 4); /* Month */
	write(1, time_str + 8, 3); /* Day */

	if (*last_update < now - 15768000 || *last_update > now + 60)
	{
		write(1, " " , 1); /* Spacing */
		write(1, time_str + 20 , 4); /* Year */
	}
	else
		write(1, time_str + 11 , 5); /* Time */
}

void link_n_display(int padding, __nlink_t link_n)
{
	padding -= ft_digitlen(link_n);
	for(int spaces = 0; spaces < padding; spaces++)
		ft_putchar_fd(' ', 1);
	ft_put_unnbr_fd(link_n, 1);
}

void owner_display(int padding, __uid_t uid)
{
	static const struct passwd* user_info = NULL;
	if (!user_info || user_info->pw_gid != uid)
		user_info = getpwuid(uid);
	if (user_info)
	{
		ft_putstr_fd(user_info->pw_name, 1);
		padding -= ft_strlen(user_info->pw_name);
		for(int spaces = 0; spaces < padding; spaces++)
			ft_putchar_fd(' ', 1);
	}
	else
	{
		ft_put_unnbr_fd(uid, 1);
		padding -= ft_digitlen(uid);
		for(int spaces = 0; spaces < padding; spaces++)
			ft_putchar_fd(' ', 1);
	}
}

void group_display(int padding, __gid_t gid)
{
	static const struct group* group_info = NULL;
	if (!group_info || group_info->gr_gid != gid)
		group_info = getgrgid(gid);
	if (group_info)
	{
		ft_putstr_fd(group_info->gr_name, 1);
		padding -= ft_strlen(group_info->gr_name);
		for(int spaces = 0; spaces < padding; spaces++)
			ft_putchar_fd(' ', 1);
	}
	else
	{
		ft_put_unnbr_fd(gid, 1);
		padding -= ft_digitlen(gid);
		for(int spaces = 0; spaces < padding; spaces++)
			ft_putchar_fd(' ', 1);
	}
}
void size_display(int padding, __off_t size)
{
	padding -= ft_digitlen(size);
	for(int spaces = 0; spaces < padding; spaces++)
		ft_putchar_fd(' ', 1);
	ft_put_unnbr_fd(size, 1);
}


void display_item(t_item *file, bool flag_list, t_list_padding padding, char path[PATH_MAX])
{
	if(flag_list)
	{
		const struct stat item_stat = file->item_stat;

		stats_display(item_stat.st_mode);  /* Mode */

		ft_putchar_fd(' ', 1); /* Links number */
		link_n_display(padding.link, item_stat.st_nlink);

		ft_putchar_fd(' ', 1); /* Owner User */
		owner_display(padding.user, item_stat.st_uid);

		ft_putchar_fd(' ', 1); /* Group Owner */
		group_display(padding.group, item_stat.st_gid);

		ft_putchar_fd(' ', 1); /* Size of the file */
		size_display(padding.size, item_stat.st_size);

		ft_putchar_fd(' ', 1); /* Date */
		time_display(item_stat.st_mtim);

		ft_putstr_fd(" ", 1); /* File name */
		ft_putstr_fd(file->pathname, 1);
		if (S_ISLNK(item_stat.st_mode))
		{
			ft_putstr_fd(" -> ", 1);
			char link_path[PATH_MAX] = {};
			char link_result[PATH_MAX] = {};
			ft_memcpy(link_path , path, ft_strlen(path));
			if (link_path[ft_strlen(link_path) - 1] != '/')
				link_path[ft_strlen(link_path)] = '/';
			ft_memcpy(link_path + ft_strlen(link_path), file->pathname, ft_strlen(file->pathname));
			if (readlink(link_path, link_result, PATH_MAX) == -1)
			{
				ft_putstr_fd("ft_ls: ", 2);
				perror(link_path);
				return; // todo : error ?
			}
			ft_putstr_fd(link_result, 1);
		}
	}
	else
	{
		ft_putstr_fd(file->pathname, 1);
	}
}

t_list_padding get_padding(t_list *items, bool flag_list)
{
	t_list_padding padding = {0,0,0,0};
	if (!flag_list)
		return padding;
	while (items)
	{
		t_item *item = items->content;

		int link = ft_digitlen(item->item_stat.st_nlink);
		if (link > padding.link)
			padding.link = link;

		int size = ft_digitlen(item->item_stat.st_size);
		if (size > padding.size)
			padding.size = size;

		static struct passwd* user_info = NULL;
		if (!user_info || user_info->pw_gid != item->item_stat.st_uid)
			user_info = getpwuid(item->item_stat.st_uid);
		int user = 0;
		if (user_info)
			user = ft_strlen(user_info->pw_name);
		else
			user = ft_digitlen(item->item_stat.st_uid);
		if (user > padding.user)
			padding.user = user;

		static const struct group* group_info = NULL;
		if (!group_info || group_info->gr_gid != item->item_stat.st_gid)
			group_info = getgrgid(item->item_stat.st_gid);
		int group = 0;
		if (group_info)
			group = ft_strlen(group_info->gr_name);
		else
			group = ft_digitlen(item->item_stat.st_gid);
		if (group > padding.group)
			padding.group = group;

		items = items->next;
	}
	return padding;
}

void display_folder_content(t_item* dir, t_flags flags, bool show_name_folder, bool first)
{
	t_list *item_to_print = get_items_from_folder(dir->pathname, flags.all);
	if (item_to_print == (void*) -1)
		return;
	if (show_name_folder || flags.recursive)
	{
		if (!first)
			ft_putchar_fd('\n', 1);
		first = false;
		ft_printf("%s:\n", dir->pathname);
	}
	if (flags.list)
	{
		ft_putstr_fd("total ", 1);
		t_list *root_item = item_to_print;
		long int total = 0;
		while (item_to_print)
		{
			t_item *item = item_to_print->content;
			if (ft_memcmp(item->pathname, ".", 2) != 0 && ft_memcmp(item->pathname, "..", 3) != 0)
				total += item->item_stat.st_blocks;  // By man 7 inode : the size of a block is not defined by POSIX.1 so it may be 1024.
			item_to_print = item_to_print->next;
		}
		ft_putnbr_fd(total / 2, 1); // i don't know why but ls seems to print for 1k blocks, tho POSIX define them as size of 512.
		ft_putchar_fd('\n', 1);
		item_to_print = root_item;
	}
	sort_items(&item_to_print, flags.time, flags.reverse);
	t_list *root_item = item_to_print;
	const t_list_padding padding = get_padding(item_to_print, flags.list);
	while (item_to_print) // first we print the items
	{
		display_item(item_to_print->content, flags.list, padding, dir->pathname);
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
		t_list_padding padding = get_padding(files, flags.list);
		char path[PATH_MAX] = {};
		path[0] = '.';
		while(files)
		{
			display_item(files->content, flags.list, padding, path);
			if (!flags.list && files->next)
				write(1, "  ", 2);
			files = files->next;
			if (flags.list)
				ft_putchar_fd('\n', 1);
		}
		show_name_folder = true;
		first = false;
		if (chain_items.dirs)
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

			dir = dir->next;
		}
	}
}
