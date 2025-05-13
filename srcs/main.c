#include <stdio.h>

#include "../incs/ft_ls.h"

int main(){
	// char **file_list;

	DIR *dir;
	struct dirent* dirent;
	dir = opendir(".");
	while ((dirent = readdir(dir)) != NULL)
	{
		printf("%s  ", dirent->d_name);
	}
	closedir(dir);
	;
}