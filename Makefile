NAME		=	philo

SRCS		=	philo.c \
				init.c \
				validate.c \
				philo_utils.c \
				mutex.c

OBJS		=	${SRCS:.c=.o}

CFLAGS		=	-Wall -Wextra -Werror -g -pthread

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	@rm -f $(OBJS)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all, clean, fclean, re