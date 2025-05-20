#include "../incs/ft_ls.h"

void get_file(char *filename)
{
	struct stat statbuf;
	stat(filename,&statbuf);
	printf("hello :D\n");
}

int main(int argc, char **argv){
	DIR *dir;
	struct dirent* dirent;

	if (argc == 1)
		dir = opendir(".");
	else
		dir = opendir(argv[1]);

	while ((dirent = readdir(dir)) != NULL)
	{
		struct stat statbuf;
		stat(dirent->d_name,&statbuf);
		printf("owner of %s->%s\n", dirent->d_name, getpwuid(statbuf.st_uid)->pw_passwd);
		// printf("%s  ", dirent->d_name);
		;
	}
	printf("\n");
	closedir(dir);
}