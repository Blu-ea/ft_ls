#include <stdio.h>

#include "../incs/ft_ls.h"


int main(int argc, char **argv){
	DIR *dir;
	struct dirent* dirent;

	if (argc == 1)
		dir = opendir(".");
	else
		dir = opendir(argv[1]);

	while ((dirent = readdir(dir)) != NULL)
	{
		printf("%s  ", dirent->d_name);
	}
	printf("\n");
	closedir(dir);
}