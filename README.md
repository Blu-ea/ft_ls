# ft_ls
This is a reproduction of the unix ls command in C 

# What it does

`ls` list "all" the files present in a certain directory.

By default, the hidden files will not be showed (see [-a](#flag--a)).
The files will be sorted alphabetically by default (see [-t](#flag--t) and [-r](#flag--r-1)).

---

# Flags Required
 - [`-R`](#flag--r)
 - [`-l`](#flag--l)
 - [`-a`](#flag--a)
 - [`-r`](#flag--r-1)
 - [`-t`](#flag--t)

## Flags 

### - Flag `-R`

Recursive, will enter all sub directory and list all files and directory in those sub directories

### - Flag `-l`

List, will list the files recorded in a table, such as : <br>

| File mode  | Number of links | Owner name | Group name | Size | Date and time of last editing | Pathname  |
|------------|-----------------|------------|------------|------|-------------------------------|-----------|
| -rw-r--r-- | 1               | Blu-ea     | Blu-ea     | 463  | 13 mai  01:56                 | README.md |

If the file is a symbolic links, the Pathname will be like `<link_name> -> <path_to_linked_file>`


### - Flag `-a`

All, the list of file listed will contain all file, including the hiden one ( exemple: `.env` )

### - Flag `-r`

Reverse, the list order will be reverse. (show the bottom of the list first)

### - Flag `-t`

Time, will sort the list by time, aka.`newest first` 

---

# Authorised function
 - write
 - opendir
 - readdir
 - closedir
 - stat
 - lstatid
 - getgrgid
 - getpwu
 - listxattr
 - getxattr
 - time
 - ctime
 - readlink
 - malloc
 - free
 - perror
 - strerror
 - exit