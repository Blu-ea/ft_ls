//
// Created by Blu-ea on 06/06/25.
//

#include "../incs/ft_ls.h"

bool compare_time(t_item* a, t_item* b);
bool compare_name(t_item* a, t_item* b);

void sort_items(t_list** lst, bool flag_time, bool flag_reverse)
{
	if (!lst)
		return;
	bool (*compare)(t_item* a, t_item* b);
	if (flag_time)
		compare = compare_time;
	else
		compare = compare_name;
	// t_list* tmp;
	t_list* previous;
	t_list* it;
	t_list* it2;
	previous = NULL;
	it = *lst;
	it2 = (*lst)->next;
	while (it2 != NULL)
	{
		if (compare(it->content, it2->content) != flag_reverse)
		{
			if (previous == NULL)
				*lst = it2;
			else
				previous->next = it2;

			it->next = it2->next;
			it2->next = it;

			previous = NULL;
			it = *lst;
			it2 = it->next;
		}
		else
		{
			previous = it;
			it = it->next;
			it2 = it2->next;
		}
	}
}

bool compare_time(t_item* a, t_item* b)
{
	if (a->item_stat.st_mtim.tv_sec < b->item_stat.st_mtim.tv_sec ||
		(a->item_stat.st_mtim.tv_sec == b->item_stat.st_mtim.tv_sec && a->item_stat.st_mtim.tv_nsec < b->item_stat.st_mtim.tv_nsec))
		return true;
	return false;
}
bool compare_name(t_item* a, t_item* b)
{
	if (ft_strcmp(a->pathname, b->pathname) > 0)
		return true;
	return false;
}