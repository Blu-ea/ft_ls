#include "../incs/ft_ls.h"


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
		if (_r_content->d_type == DT_DIR
			&& (_r_content->d_name[0] != '.'  || all_flag)
			&& strcmp(_r_content->d_name, ".") != 0
			&& strcmp(_r_content->d_name, "..") != 0)
		{
			t_item* current_item;
			current_item = ft_calloc(1, sizeof(t_item));
			char fullpath[PATH_MAX];
			ft_memset(fullpath, 0, PATH_MAX);
			ft_memcpy(fullpath, folder_path, ft_strlen(folder_path));
			if (fullpath[ft_strlen(folder_path) - 1] != '/')
				fullpath[ft_strlen(folder_path)] = '/';
			ft_memcpy(fullpath + ft_strlen(fullpath), _r_content->d_name, ft_strlen( _r_content->d_name) );
			current_item->pathname = ft_strdup(fullpath);
			stat(fullpath, &current_item->item_stat);
			t_list *new_item = ft_lstnew(current_item);
			if (!new_item)
			{
				ft_putstr_fd("ft_ls: out of memory", 2);
				return;
			}
			ft_lstadd_back(parm_dir, new_item);
			recursive_get(parm_dir, fullpath, all_flag);
		}
	}
	closedir(dir);
}

t_ls_lst_parms get_parms(char **path, const bool recursive, const bool all_flag)
{
	t_ls_lst_parms parm = {NULL, 0,NULL};
	t_item* current_item;
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

		current_item = ft_calloc(1, sizeof(t_item));
		current_item->pathname = path[i];
		current_item->item_stat = current_stats;

		if (S_ISDIR(current_stats.st_mode))
		{
			ft_lstadd_front(&parm.dirs, ft_lstnew(current_item));
			(void) recursive;
			(void) all_flag;
			// if (recursive) // todo: Recursivity need to be done during the rendering
				// recursive_get(&parm.dirs , path[i], all_flag);
		}
		else
		{
			ft_lstadd_front(&parm.files, ft_lstnew(current_item));
			size_t n = current_stats.st_size;
			size_t len = 1;
			while (n >= 10) {
				n /= 10;
				len++;
			}
			if (len > parm.max_dg_lenght) parm.max_dg_lenght = len;
		}
	}
	return parm;
}

void print_file(t_item *temp_item);

t_list *get_items_from_folder(char *pathname, bool flag_all)
{
	size_t pathname_len = ft_strlen(pathname);
	char fullpath[PATH_MAX];
	t_item* current_item;
	DIR *dir = opendir(pathname);
	if (!dir)
	{
		ft_putstr_fd("ft_ls: cannot access `", 2);
		ft_putstr_fd(pathname, 2);
		perror("`");
		return NULL;
	}
	t_list* list_folder = NULL;
	ft_memset(fullpath, '\0', PATH_MAX);
	ft_memcpy(fullpath, pathname, pathname_len);
	if (fullpath[pathname_len] != '/')
	{
		fullpath[pathname_len] = '/';
		fullpath[++pathname_len] = 0;
	}

	for (struct dirent* _r_content = readdir(dir); _r_content; _r_content = readdir(dir))
	{
		if (_r_content->d_name[0] == '.'  && !flag_all)
			continue;
		ft_memcpy(fullpath + pathname_len, _r_content->d_name, ft_strlen(_r_content->d_name) + 1);
		struct stat file_stats;
		if (stat(fullpath, &file_stats) == -1)
		{
			// printf(fullpath);
			// printf(" | %s   | %s-\n", pathname , _r_content->d_name);
			continue;
		}

		current_item = ft_calloc(1, sizeof(t_item));
		if (!current_item)
		{
			ft_putstr_fd("ft_ls: out of memory", 2);
			return NULL; // todo : clear the t_list*
		}

		current_item->pathname = ft_strdup(_r_content->d_name);
		current_item->item_stat = file_stats;

		t_list *new_item = ft_lstnew(current_item);
		if (!new_item)
		{
			ft_putstr_fd("ft_ls: out of memory", 2);
			return NULL; // todo: clear the t_list*
		}
		ft_lstadd_front(&list_folder, new_item);
	}
	closedir(dir);
	return list_folder;
}
