//
// Created by Blu-ea on 06/06/25.
//

#include "../incs/ft_ls.h"

static bool compare_time(t_item* a, t_item* b, bool flag_reverse);
static bool compare_name(t_item* a, t_item* b, bool flag_reverse);

typedef bool (*compare_func)(t_item* a, t_item* b, bool flag_reverse);

// void sort_items(t_list** lst, bool flag_time, bool flag_reverse)
// {
// 	if (!lst || !*lst)
// 		return;
// 	compare_func compare;
//
// 	if (flag_time)
// 		compare = compare_time;
// 	else
// 		compare = compare_name;
// 	t_list* previous;
// 	t_list* it;
// 	t_list* it2;
// 	previous = NULL;
// 	it = *lst;
// 	it2 = (*lst)->next;
// 	while (it2 != NULL)
// 	{
// 		if (compare(it->content, it2->content, flag_reverse))
// 		{
// 			if (previous == NULL)
// 				*lst = it2;
// 			else
// 				previous->next = it2;
//
// 			it->next = it2->next;
// 			it2->next = it;
//
// 			previous = NULL;
// 			it = *lst;
// 			it2 = it->next;
// 		}
// 		else
// 		{
// 			previous = it;
// 			it = it->next;
// 			it2 = it2->next;
// 		}
// 	}
// }

static t_list* split(t_list* head, int n) {
	while (--n && head)
		head = head->next;
	if (!head)
		return NULL;
	t_list* rest = head->next;
	head->next = NULL;
	return rest;
}

static t_list* merge(t_list* a, t_list* b, compare_func compare, bool flag_reverse) {
	t_list* head = NULL;
	t_list** tailRef = &head;

	while (a && b) {
		if (compare (a->content, b->content, flag_reverse)) {
			*tailRef = b;
			b = b->next;
		} else {
			*tailRef = a;
			a = a->next;
		}
		tailRef = &((*tailRef)->next);
	}

	*tailRef = a ? a : b;
	return head;
}

void sort_items_merge(t_list** lst, bool flag_time, bool flag_reverse) {
	if (!lst || !*lst)
		return;
	bool (*compare)(t_item* a, t_item* b, bool flag_reverse);
	if (flag_time)
		compare = compare_time;
	else
		compare = compare_name;

	int length = ft_lstsize(*lst);

	for (int size = 1; size < length; size *= 2) {
		t_list** tailRef = lst;
		t_list* curr = *lst;

		while (curr) {
			t_list* left = curr;
			t_list* right = split(left, size);
			curr = split(right, size);
			t_list* merged = merge(left, right, compare, flag_reverse);

			*tailRef = merged;
			while (*tailRef)
				tailRef = &((*tailRef)->next);
		}
	}
}

static bool compare_time(t_item* a, t_item* b, bool flag_reverse)
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

static bool compare_name(t_item* a, t_item* b, bool flag_reverse)
{
	int const cmp = ft_strcmp(a->pathname, b->pathname);
	if (cmp == 0)
		return false;
	if (cmp > 0)
		return !flag_reverse;
	return flag_reverse;
}