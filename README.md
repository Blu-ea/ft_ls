# FT_LS
This is a reproduction of the unix ls command in C 

# What it does

`ls` list "all" the files present in a certain directory.

By default, the hidden files will not be showed (see [-a](#flag--a)).
The files will be sorted by byte-by-byte (by the ASCII table) by default (see [-t](#flag--t) and [-r](#flag--r-1)).  

There is also color for directories, links, and executables. If the link's target is not valid, then the color will be in red


> [!NOTE]  
> See `--help` to see the usage.
> 
> Usage: `./ft_ls [OPTION] [FOLDER]`
> 
> Options:
> <dl>
> <code>-a</code>	 <dd>Write out all directory entries, including those whose names begin with a &lt;period&gt; ('.').</dd>
> <code>-A</code>	 <dd>Write out all directory entries, including those whose names begin with a &lt;period&gt; ('.') but excluding the entries dot and dot-dot (if they exist).</dd>
> <code>-l</code>	 <dd>Write out in long format.</dd>
> <code>-r</code>	 <dd>Reverse the order of the sort to get reverse collating sequence oldest first, or smallest file size first depending on the other options given.</dd>
> <code>-R</code>    <dd>Recursively list subdirectories encountered. When a symbolic link to a directory is encountered, the directory shall not be recursively listed.</dd>
> <code>-t</code>	 <dd>Sort with the primary key being time modified (most recently modified first) and the secondary key being filename in the collating sequence.  For a symbolic link, the time used as the sort key is that of the symbolic link itself.</dd>
> <code>-U</code>	 <dd>Do not sort directory entries.</dd>
> <code>--help</code>	<dd> display this help and exit.</dd>
> </dl>


---

# Flags Required
 - [`-a`](#--flag--a)
 - [`-A`](#--flag--a-1)
 - [`-l`](#--flag--l)
 - [`-r`](#--flag--r)
 - [`-R`](#--flag--r-1)
 - [`-t`](#--flag--t)
 - [`-U`](#--flag--u)

## Flags 

### - Flag `-a`

All, the list of file listed will contain all file, including the hiden one ( exemple: `.env` )

### - Flag `-A`

Almost, the list of file listed will contain almost all the file, being the hidden one, but not the `.` and `..`, that are contain in [`-a`](#--flag--a)

### - Flag `-l`

List, will list the files recorded in a table, such as : <br>

| File mode  | Number of links | Owner name | Group name | Size | Date and time of last editing | Pathname  |
|------------|-----------------|------------|------------|------|-------------------------------|-----------|
| -rw-r--r-- | 1               | Blu-ea     | Blu-ea     | 463  | 13 mai  01:56                 | README.md |

If the file is a symbolic links, the Pathname will be like `<link_name> -> <path_to_linked_file>`

### - Flag `-r`

Reverse, the list order will be reverse. (show the bottom of the list first)

### - Flag `-R`

Recursive, will enter all sub directory and list all files and directory in those sub directories

### - Flag `-t`

Time, will sort the list by time, aka.`newest first` 

### - Flag `-U`

Unsort, the files will not be preemptively sorted, and will be ordered as the in directory order
