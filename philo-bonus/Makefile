NAME =	philo

CC = cc
CFLAGS = -g
# CFLAGS = -Wall -Wextra -Werror -g
RM = rm -rf
HEAD = push_swap.h
SRCS = 	main.c\

SRCS_BONUS =\

OBJS = $(SRCS:.c=.o)
OBJS_BONUS = $(SRCS_BONUS:.c=.o)
TOTAL_FILES = $(words $(SRCS))

all:			$(NAME)
#				@clear
				@echo "Compilation terminée";

$(NAME):		$(OBJS)
				@$(CC) -o $(NAME) $(OBJS) $(CFLAGS)
.c.o:
	@$(CC) $(CFLAGS) -c $< -o $@

bonus :			$(OBJS_BONUS)
				@$(CC) -o $(NAME) $(OBJS_BONUS) $(CFLAGS)
				@echo "Compilation terminée";

clean:			
				@$(RM) $(OBJS)
#				@clear
				@echo "Clean OK";


fclean: 		clean
#				@clear
				@echo "Full Clean OK ";

re: 			fclean all

.PHONY:			all clean fclean re