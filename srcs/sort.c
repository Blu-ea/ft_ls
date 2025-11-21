//
// Created by Blu-ea on 06/06/25.
//

#include "../incs/ft_ls.h"

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

void sort_items_merge(t_list** lst, compare_func compare, bool flag_reverse) {
	if (!lst || !*lst)
		return;

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

bool no_sort(t_item* a, t_item* b, bool flag_reverse) {
	(void) a;
	(void) b;
	(void)flag_reverse;
	return true;
}