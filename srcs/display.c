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

void print_ls_time(struct timespec ts)
{
	const time_t *last_update = &ts.tv_sec;
	const time_t now = time(NULL);

	char *time_str = ctime(last_update);
	write(1, time_str + 4, 4); /* Month */
	write(1, time_str + 8, 3); /* Day */

	if (*last_update < now - 15552000 || *last_update > now + 60)
		write(1, time_str + 20 , 4); /* Year */
	else
		write(1, time_str + 11 , 5); /* Time */
}

void display_item(t_item *file, bool flag_list, size_t space_size)
{
	if(flag_list)
	{
		const struct stat f_stat = file->item_stat;
		stat_type(f_stat.st_mode); /* Mode */
		stat_display(S_IRUSR, f_stat.st_mode, 'r');
		stat_display(S_IWUSR, f_stat.st_mode, 'w');
		stat_display(S_IXUSR, f_stat.st_mode, 'x');

		stat_display(S_IRGRP, f_stat.st_mode, 'r');
		stat_display(S_IWGRP, f_stat.st_mode, 'w');
		stat_display(S_IXGRP, f_stat.st_mode, 'x');

		stat_display(S_IROTH, f_stat.st_mode, 'r');
		stat_display(S_IWOTH, f_stat.st_mode, 'w');
		stat_display(S_IXOTH, f_stat.st_mode, 'x');

		ft_putchar_fd(' ', 1); /* Links number */
		ft_put_unnbr_fd(f_stat.st_nlink, 1);

		ft_putchar_fd(' ', 1); /* Owner User */
		const struct passwd* user_info = getpwuid(f_stat.st_uid);
		if (user_info)
			ft_putstr_fd(user_info->pw_name, 1);
		else
			ft_put_unnbr_fd(f_stat.st_uid, 1);

		ft_putchar_fd(' ', 1); /* Group Owner */
		const struct group* group_info = getgrgid(f_stat.st_gid);
		if (group_info)
			ft_putstr_fd(group_info->gr_name, 1);
		else
			ft_put_unnbr_fd(f_stat.st_gid, 1);

		ft_putchar_fd(' ', 1); /* Size of the file */
		space_size -= ft_digitlen(f_stat.st_size);
		for(size_t spaces = 0; spaces < space_size; spaces++)
			ft_putchar_fd('.', 1);
		ft_put_unnbr_fd(f_stat.st_size, 1);

		ft_putchar_fd(' ', 1); /* Date */
		print_ls_time(f_stat.st_mtim);

		ft_putchar_fd(' ', 1); /* File name */
		ft_putstr_fd(file->pathname, 1); // todo : handles links
	}
	else
	{
		ft_printf("%s  ", file->pathname);
	}
}
void print_file(t_list *);

void display_ls(t_ls_lst_parms chain_items, t_flags flags)
{
	bool first = true;
	size_t show_name_folder = false;
	if (chain_items.files)
	{
		t_list *files = chain_items.files;
		while(files)
		{
			display_item(files->content, flags.list, chain_items.max_dg_lenght);
			files = files->next;
			if (flags.list)
				ft_putchar_fd('\n', 1);
		}
		show_name_folder = true;
		first = false;
		ft_putchar_fd('\n', 1);
	}
	if (chain_items.dirs)
	{
		t_list* dir = chain_items.dirs;
		show_name_folder = !!(ft_lstsize(dir) - 1) || show_name_folder;
		while(dir)
		{
			// printf("test1?\n");
			// ft_lstiter(dir, (void (*)(void *))print_file);
			t_list *item_to_print = get_items_from_folder(((t_item*)dir->content)->pathname, flags.all);
			// printf("test2?\n");
			if (!item_to_print)
			{
				dir = dir->next;
				continue;
			}
			if (show_name_folder)
			{
				if (!first)
					ft_putchar_fd('\n', 1);
				first = false;
				ft_printf("%s:\n", ((t_item*)dir->content)->pathname);
			}
			while (item_to_print)
			{
				display_item(item_to_print->content, flags.list, 10);
				item_to_print = item_to_print->next;
				if (flags.list)
					ft_putchar_fd('\n', 1);
			}
			ft_putchar_fd('\n', 1);
			dir = dir->next;
		}
	}
}
