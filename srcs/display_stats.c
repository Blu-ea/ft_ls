//
// Created by Blu-ea on 17/08/2025.
//

#include "../incs/ft_ls.h"


static void stat_display(unsigned int mode_mask, __mode_t mode_value, char print_flag)
{
	if( mode_mask & mode_value)
		write(1, &print_flag,1);
	else
		write(1, "-", 1);
}


static void stat_type(__mode_t mode_value)
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

static void stats_display(__mode_t mode_value)
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

static void time_display(struct timespec ts)
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

static void link_n_display(int padding, __nlink_t link_n)
{
	padding -= ft_digitlen(link_n);
	for(int spaces = 0; spaces < padding; spaces++)
		ft_putchar_fd(' ', 1);
	ft_put_unnbr_fd(link_n, 1);
}

static void owner_display(int padding, __uid_t uid)
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

static void group_display(int padding, __gid_t gid)
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

static void size_display(int padding, __off_t size)
{
	padding -= ft_digitlen(size);
	for(int spaces = 0; spaces < padding; spaces++)
		ft_putchar_fd(' ', 1);
	ft_put_unnbr_fd(size, 1);
}


void display_item_stats(t_item *file, bool flag_list, t_list_padding padding, char path[PATH_MAX])
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
}
