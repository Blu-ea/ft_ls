NAME := ft_ls

# **************************************************************************** #
DIR_SRCS	:= srcs
DIR_OBJS	:= .objs
DIR_INCS	:= incs

# **************************************************************************** #
LST_SRCS	:=	main.c\
				get_file.c\
				parsing.c\
				display.c\
				display_stats.c\
				sort.c\
				padding.c

LST_OBJS	:=	$(LST_SRCS:.c=.o)

LST_INCS	:=	ft_ls.h\
				dir_content.h

# **************************************************************************** #
SRCS		:=	$(addprefix $(DIR_SRCS)/,$(LST_SRCS))
OBJS		:=	$(addprefix $(DIR_OBJS)/,$(LST_OBJS))
INCS		:=	$(addprefix $(DIR_INCS)/,$(LST_INCS))

# **************************************************************************** #
CC			:=	gcc
CFLAGS		:=	-Wall -Wextra -Werror -g3 # -fsanitize=address

# **************************************************************************** #
ERASE	:=	\033[2K\r
GREY	:=	\033[30m
RED		:=	\033[31m
GREEN	:=	\033[32m
YELLOW	:=	\033[33m
BLUE	:=	\033[34m
PINK	:=	\033[35m
CYAN	:=	\033[36m
WHITE	:=	\033[37m
BOLD	:=	\033[1m
UNDER	:=	\033[4m
SUR		:=	\033[7m
END		:=	\033[0m

# **************************************************************************** #
all : $(NAME)

$(NAME) : libft $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) libft/libft.a -o $@

$(DIR_OBJS)/%.o: $(DIR_SRCS)/%.c $(INCS) Makefile | $(DIR_OBJS)
	$(CC) $(CFLAGS) -c $< -o $@ -I$(DIR_INCS) -Ilibft/includes

$(DIR_OBJS) :
	mkdir -p $(DIR_OBJS)

libft :
	git submodule init
	git submodule update --remote --merge
	$(MAKE) -C ./libft

clean :
	rm -rf $(DIR_OBJS)

fclean :
	rm -f $(NAME)
	rm -rf $(DIR_OBJS)

re : fclean all

.PHONY : all bonus clean fclean re libft
# .SILENT :