//
// Created by Blu-ea on 30/06/25.
//

#include "../incs/ft_ls.h"

size_t get_size(size_t size, bool update)
{
	static size_t size_max = 0;
	if (!update)
	{
		size = size_max;
		size_max = 0;
		return size;
	}
	if (size_max < size)
		size_max = size;
	printf("size_max: %zu\n", size_max);
	return 0;
}

void ft_set_size(void *item)
{
	get_size(ft_strlen(((t_item*)item)->pathname), true);
}

// unused
void get_term_width(t_list *files, size_t lst_size)
{
	ft_lstiter(files, ft_set_size);
	size_t max_len = get_size(0, false);

	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	int term_width = w.ws_col;

	int col_width = max_len + 2;
	int cols = term_width / col_width;
	if (cols < 1) cols = 1;

	int rows = (lst_size + cols - 1) / cols;
	t_item *items[lst_size];
	size_t i = 0;
	while (files)
	{
		items[i] = (t_item*)files->content;
		files = files->next;
		i++;
	}

	// printf("rows = %d\n", rows);
	// printf("cols = %d\n", cols);
	// printf("cols_W = %d\n", col_width);
	for (int r = 0; r < rows; r++) {
		for (int c = 0; c < cols; c++) {
			size_t idx = c * rows + r;

			if (idx < lst_size) {
				// printf("%-*s", col_width, items[idx]->pathname);
				size_t padding = col_width - ft_strlen(items[idx]->pathname);
				write(1, items[idx]->pathname, strlen(items[idx]->pathname));
				size_t i = -1;
				while (++i < padding)
					write(1, " ", 1);
			}
		}
		// printf("\n");
	}
}

int get_column_width(int *all_size, int nb_of_item, int current_column, int line_count)
{
	int column_size = 0;
	int it = current_column * line_count;
	int end = it + line_count;
	while (it < end && it < nb_of_item)
	{
		if (column_size < all_size[it])
			column_size = all_size[it];
		it ++;
	}
	return column_size;
}

size_t calc_column_size(t_list *files, size_t max_column, int *column_size)
{
	int all_size[max_column];
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	const int term_width = w.ws_col;

	size_t i = 0;
	while (files)
	{
		all_size[i] = ft_strlen(((t_item*)files->content)->pathname) + 2;
		files = files->next;
		i++;
	}
	i = 0;
	size_t column_count = max_column;
	int line_lenght = 0;
	size_t line_count = 1;
	while (i < column_count)
	{
		column_size[i] = get_column_width(all_size, max_column, i , line_count) ;
		line_lenght += column_size[i];

		if (line_lenght - 2 >= term_width)
		{
			line_count ++;
			column_count =(max_column + line_count - 1) / line_count;
			line_lenght = 0;
			i = 0;
		}
		else
			i++;
	}
	column_size[i] = 0;
	return column_count;
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
