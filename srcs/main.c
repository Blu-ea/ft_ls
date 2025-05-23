#include "../incs/ft_ls.h"

void get_file(char *filename)
{
	struct stat statbuf;
	stat(filename,&statbuf);
	printf("hello :D\n");
}

int main(int argc, char **argv){
	t_flags flags = parsing(argc, argv);
	print_flags(flags);
	ft_free_2d_array(flags.paths);
}
