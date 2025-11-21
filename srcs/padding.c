//
// Created by Blu-ea on 30/06/25.
//

#include "../incs/ft_ls.h"

// size_t get_size(size_t size, bool update)
// {
// 	static size_t size_max = 0;
// 	if (!update)
// 	{
// 		size = size_max;
// 		size_max = 0;
// 		return size;
// 	}
// 	if (size_max < size)
// 		size_max = size;
// 	printf("size_max: %zu\n", size_max);
// 	return 0;
// }
//
// void ft_set_size(void *item)
// {
// 	get_size(ft_strlen(((t_item*)item)->pathname), true);
// }

static int    get_column_width(int *all_size, int nb_of_item, int current_column, size_t line_count)
{
	int column_size = 0;
	int it = current_column * line_count;
	const int end = it + line_count;
	while (it < end && it < nb_of_item)
	{
		if (column_size < all_size[it])
			column_size = all_size[it];
		it ++;
	}
	return column_size;
}

size_t calc_column_size(const t_list *files, const size_t max_column, size_t *column_size, size_t *line_count)
{
	struct winsize w = {0};
	size_t term_width = 0;
	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) != -1)
		term_width = w.ws_col;

	int all_size[max_column]; // The lenght of all the file PLUS 2 for the separator
	size_t i = 0;
	while (files)
	{
		all_size[i] = ft_strlen(((t_item*)files->content)->pathname) + 2;
		files = files->next;
		i++;
	}
	i = 0;
	size_t column_count = max_column;
	size_t line_lenght = 0;
	while (i < column_count)
	{
		column_size[i] = get_column_width(all_size, max_column, i , *line_count) ;
		if (column_count == 1)
			break;
		line_lenght += column_size[i];

		if (line_lenght - 1 >= term_width) // Minus 2 to remove the last separator
		{
			(*line_count) ++;  // We did not find so we increment the number of possible line
			column_count = (max_column + *line_count - 1) / *line_count; // We recalculate the number of column needed
			line_lenght = 0;
			i = 0;
		}
		else
			i++;
	}
	if (i != 0)
		column_size[i] = 0;
	return column_count;
}

t_list_padding get_padding_list_flag(t_list *items, bool flag_list)
{
	t_list_padding padding = {0,0,0,0};
	if (!flag_list)
		return padding;
	while (items)
	{
		t_item *item = items->content;

		const int link = ft_digitlen(item->item_stat.st_nlink);
		if (link > padding.link)
			padding.link = link;

		const int size = ft_digitlen(item->item_stat.st_size);
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
