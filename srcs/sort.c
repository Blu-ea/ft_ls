//
// Created by Blu-ea on 06/06/25.
//

#include "../incs/ft_ls.h"

bool compare_time(t_item* a, t_item* b, bool flag_reverse);
bool compare_name(t_item* a, t_item* b, bool flag_reverse);

void sort_items(t_list** lst, bool flag_time, bool flag_reverse)
{
	if (!lst || !*lst)
		return;
	bool (*compare)(t_item* a, t_item* b, bool flag_reverse);
	if (flag_time)
		compare = compare_time;
	else
		compare = compare_name;
	t_list* previous;
	t_list* it;
	t_list* it2;
	previous = NULL;
	it = *lst;
	it2 = (*lst)->next;
	while (it2 != NULL)
	{
		if (compare(it->content, it2->content, flag_reverse))
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

bool compare_time(t_item* a, t_item* b, bool flag_reverse)
{
	long int const a_sec = a->item_stat.st_mtim.tv_sec;
	long int const b_sec = b->item_stat.st_mtim.tv_sec;
	long int const a_nsec = a->item_stat.st_mtim.tv_nsec;
	long int const b_nsec = b->item_stat.st_mtim.tv_nsec;
	if (a_sec == b_sec && a_nsec == b_nsec)
		return compare_name(a, b, flag_reverse);

	if (a_sec < b_sec || (a_sec == b_sec && a_nsec < b_nsec))
		return !flag_reverse;
	return flag_reverse;
}

bool compare_name(t_item* a, t_item* b, bool flag_reverse)
{
	int const cmp = ft_strcmp(a->pathname, b->pathname);
	if (cmp == 0)
		return false;
	if (cmp > 0)
		return !flag_reverse;
	return flag_reverse;
}