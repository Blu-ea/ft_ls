#ifndef FT_LS_H
# define FT_LS_H

#include <stdio.h> // printf

# include <pwd.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/stat.h>
# include <sys/ioctl.h>
# include <sys/types.h>
# include <dirent.h>

# include "dir_content.h"
#include "../libft/includes/libft.h"
#include <grp.h>
#include <time.h>

# define USAGE \
	"Usage: ./ft_ls [OPTION] [FOLDER]\n"\
	"Options:\n"\
	"	-R	Recursively list subdirectories encountered. When a symbolic link to a directory is encountered, the directory shall not be recursively listed.\n"\
	"	-a	Write out all directory entries, including those whose names begin with a <period> ('.').\n"\
	"	-l	Write out in long format.\n"\
	"	-r	Reverse the order of the sort to get reverse collating sequence oldest first, or smallest file size first depending on the other options given.\n"\
	"	-t	Sort with the primary key being time modified (most recently modified first) and the secondary key being filename in the collating sequence.\n"\
	"		For a symbolic link, the time used as the sort key is that of the symbolic link itself.\n"\
	"	--help	Display this help and exit.\n"

typedef bool (*compare_func)(t_item* a, t_item* b, bool flag_reverse);

typedef enum e_content {
	ALL,
	ALMOST,
	NO_HIDDEN,
} t_filter;

#define IS_ALL(filter) (filter == ALL)
#define IS_ALMOST(filter) (filter == ALMOST)
#define IS_NO_HIDDEN(filter) (filter == NO_HIDDEN)

typedef struct s_flags
{
	bool recursive;
	bool list;
	t_filter filter;

	bool reverse; // reverse does not affect the -U flag
	compare_func compare; // handle the Time and Unsorted flags

	bool _flag_error;
	char **paths;

}	t_flags;

t_flags	parsing(int, char**);

t_ls_lst_parms get_parms(char **path);
t_list_padding get_padding_list_flag(t_list *items, bool flag_list);
size_t calc_column_size(const t_list *files, size_t max_column, size_t *column_size, size_t *line_count);


t_list*	get_file(char **path, bool recursive);
t_list *get_items_from_folder(char *pathname, t_filter filter);

void display_ls(t_ls_lst_parms lst_parms, t_flags flags);
void display_item_stats(t_item *file, bool flag_list, t_list_padding padding, char path[PATH_MAX]);

void recursive_get(t_list** parm_dir ,char *folder_path, bool all_flag);

void get_term_width(t_list *files, size_t lst_size);

void sort_items(t_list** lst, bool flag_time, bool flag_reverse);
void sort_items_merge(t_list** lst, compare_func compare, bool flag_reverse);

// -- All the compare Functions used for sorting -- //
bool compare_time(t_item* a, t_item* b, bool flag_reverse);
bool compare_name(t_item* a, t_item* b, bool flag_reverse);
bool no_sort(t_item* a, t_item* b, bool flag_reverse);
// --  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -- //

#endif //FT_LS_H
