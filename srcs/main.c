#include "../incs/ft_ls.h"

int main(int argc, char **argv){
	const t_flags flags = parsing(argc, argv);
	if (flags.paths == NULL || flags._flag_error){
		ft_free_2d_array(flags.paths);
		return 1;
	}

	t_ls_lst_parms lst_parms = get_parms(flags.paths);
	display_ls(lst_parms, flags);

	ft_free_2d_array(flags.paths);
	ft_lstclear(&lst_parms.files, free);
	ft_lstclear(&lst_parms.dirs, free);
}
