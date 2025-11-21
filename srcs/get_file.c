#include <errno.h>

#include "../incs/ft_ls.h"

t_ls_lst_parms get_parms(char **path)
{
	t_ls_lst_parms parm = {NULL, NULL};
	t_item* current_item;
	int i = -1;

	while (path[++i])
	{
		struct stat current_stats;
		int stats_ret = lstat(path[i], &current_stats);
		if (stats_ret == -1)
		{
			ft_putstr_fd("ft_ls: cannot access `", 2);
			ft_putstr_fd(path[i], 2);
			perror("`");
			continue;
		}

		current_item = ft_calloc(1, sizeof(t_item));
		ft_memcpy(current_item->pathname, path[i], ft_strlen(path[i]));
		current_item->item_stat = current_stats;

		t_list *new_item = ft_lstnew(current_item);
		if (!new_item)
		{
			perror("ft_ls: Out of memory");
			exit(EXIT_FAILURE);
		}
		if (S_ISDIR(current_stats.st_mode))
			ft_lstadd_front(&parm.dirs, new_item);
		else
			ft_lstadd_front(&parm.files, new_item);
	}
	return parm;
}

t_list *get_items_from_folder(char *pathname, t_filter filter)
{
	size_t pathname_len = ft_strlen(pathname);
	char fullpath[PATH_MAX] = {0};
	DIR *dir = opendir(pathname);
	if (!dir)
	{
		ft_putstr_fd("ft_ls: cannot open directory `", 2);
		ft_putstr_fd(pathname, 2);
		perror("`");
		return (void*) -1;
	}
	t_list* list_folder = NULL;
	ft_memcpy(fullpath, pathname, pathname_len);
	if (fullpath[pathname_len] != '/')
	{
		fullpath[pathname_len] = '/';
		fullpath[++pathname_len] = 0;
	}

	for (struct dirent* _r_content = readdir(dir); _r_content; _r_content = readdir(dir))
	{
		if (IS_NO_HIDDEN(filter) && _r_content->d_name[0] == '.')
			continue; // We skip items starting by `.`
		if (IS_ALMOST(filter)
			&& (
				ft_memcmp(_r_content->d_name, "..", ft_strlen(_r_content->d_name)) == 0 ||
				ft_memcmp(_r_content->d_name, ".", ft_strlen(_r_content->d_name)) == 0
				))
			continue; // We skip `.` and `..`
		ft_memcpy(fullpath + pathname_len, _r_content->d_name, ft_strlen(_r_content->d_name) + 1);
		struct stat file_stats;
		if (lstat(fullpath, &file_stats) == -1)
			continue;

		t_item *current_item = ft_calloc(1, sizeof(t_item));
		if (!current_item)
		{
			perror("ft_ls: out of memory");
			ft_lstclear(&list_folder, free);
			closedir(dir);
			return (void*) -1;
		}

		ft_memcpy(current_item->pathname, _r_content->d_name, ft_strlen(_r_content->d_name));
		current_item->item_stat = file_stats;

		t_list *new_item = ft_lstnew(current_item);
		if (!new_item)
		{
			perror("ft_ls: out of memory");
			free(current_item);
			ft_lstclear(&list_folder, free);
			closedir(dir);
			return (void*) -1;
		}
		ft_lstadd_front(&list_folder, new_item);
	}
	closedir(dir);
	return list_folder;
}
