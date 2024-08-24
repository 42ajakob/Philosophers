NAME		=	philo

SRCS		=	philo.c \
				philo_utils.c \
				validate.c \
				init_philo_thr.c \
				init_tbl_mtx.c \
				mutex.c \
				check_death.c

OBJS		=	${SRCS:.c=.o}

CFLAGS		=	-Wall -Wextra -Werror -pthread -g

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	@rm -f $(OBJS)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all, clean, fclean, re
