#include "../incs/ft_ls.h"


void add_recursive(DIR* dir, t_list**);

void recursive_get(t_list** parm_dir ,char *folder_path, bool all_flag)
{
	DIR* dir = opendir(folder_path);
	if (!dir)
	{
		ft_putstr_fd("ft_ls: cannot access `", 2);
		ft_putstr_fd(folder_path, 2);
		perror("`");
		return;
	}
	for (struct dirent* _r_content = readdir(dir); _r_content; _r_content = readdir(dir))
	{
		if (_r_content->d_type == DT_DIR && (_r_content->d_name[0] != '.'  || all_flag) && strcmp(_r_content->d_name, ".") != 0 && strcmp(_r_content->d_name, "..") != 0)
		{
			t_item_list* current_item;
			current_item = calloc(1, sizeof(t_item_list));
			char *fullpath = ft_strjoin_free(ft_strjoin(folder_path,"/"), _r_content->d_name, FREE_S1);
			current_item->pathname = fullpath;
			stat(fullpath, &current_item->item_stat);
			ft_lstadd_front(parm_dir, ft_lstnew(current_item));
			recursive_get(parm_dir, fullpath, all_flag);
		}
	}
	closedir(dir);
}

t_ls_lst_parms get_parms(char **path, const bool recursive, const bool all_flag)
{
	t_ls_lst_parms parm = {NULL, NULL};
	t_item_list* current_item;
	int i = -1;

	while (path[++i])
	{
		struct stat current_stats;
		int stats_ret = stat(path[i], &current_stats);
		if (stats_ret == -1)
		{
			ft_putstr_fd("ft_ls: cannot access `", 2);
			ft_putstr_fd(path[i], 2);
			perror("`");
			continue;
		}

		current_item = calloc(1, sizeof(t_item_list));
		current_item->pathname = path[i];
		current_item->item_stat = current_stats;

		if (S_ISDIR(current_stats.st_mode))
		{
			ft_lstadd_front(&parm.dirs, ft_lstnew(current_item));
			if (recursive)
				recursive_get(&parm.dirs , path[i], all_flag);
		}
		else
			ft_lstadd_front(&parm.files, ft_lstnew(current_item));
	}
	return parm;
}


t_list* get_file(char **path, bool recursive)
{
	(void)recursive;
	t_list *files = NULL;
	t_item_list* current_item;
	int i = -1;

	while (path[++i])
	{
		current_item = calloc(1, sizeof(t_item_list));

		current_item->pathname = path[i];
		stat(path[i], &current_item->item_stat);
		// if (!current_item->exists)
		// {
			// ft_putstr_fd("ft_ls: cannot access `", 2);
			// ft_putstr_fd(path[i], 2);
			// perror("`");
			// continue;
		// }
		if (S_ISDIR(current_item->item_stat.st_mode))
		{
			ft_lstadd_back(&files, ft_lstnew(current_item));
			// if (recursive)
			// {
				// DIR* dir = opendir(current_item->pathname);
				// if (!dir)
				// {
					// ft_putstr_fd("ft_ls: cannot access `", 2);
					// ft_putstr_fd(path[i], 2);
					// perror("`");
					// continue;
				// }
			// }
		}
		else
			ft_lstadd_front(&files, ft_lstnew(current_item));
	}
	return files;
}
