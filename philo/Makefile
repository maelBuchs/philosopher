NAME =	philo

CC = cc
# CFLAGS = -g
CFLAGS = -Wall -Wextra -Werror -g
RM = rm -rf
HEAD = push_swap.h
SRCS = 	main.c\
		monitor.c\
		parsing.c\
		philosophers.c\
		time.c\
		utils.c\


OBJS = $(SRCS:.c=.o)

TOTAL_FILES = $(words $(SRCS))

all:			$(NAME)
#				@clear
				@echo "Compilation terminée";

$(NAME):		$(OBJS)
				@$(CC) -o $(NAME) $(OBJS) $(CFLAGS)
.c.o:
	@$(CC) $(CFLAGS) -c $< -o $@

clean:			
				@$(RM) $(OBJS)
#				@clear
				@echo "Clean OK";


fclean: 		clean
#				@clear
				@echo "Full Clean OK ";

re: 			fclean all

.PHONY:			all clean fclean re