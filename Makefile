##
## EPITECH PROJECT, 2017
## Makefile
## File description:
## Makefile
##

#CC	=	gcc -Wall -Wextra -O3 -g
CC	=	gcc -Wall -Wextra -g


RM	=	rm -f

NAME	=	chessformer

SRC	=	src/main.c	\
		src/helper.c	\
		src/key_check.c	\
		src/find_player.c	\
		src/map_check.c	\
		src/map_reading.c	\
		src/movement.c	\
		src/play.c	\
		src/win_check.c	\
		lib/my_putchar.c	\
		lib/my_putstr.c	\
		src/ai/hashtable.o \
		src/ai/ai.o \
		src/ai/utils.o \
		src/ai/queue.o 

CFLAGS	+=	-I./include/

OBJ	=	$(SRC:.c=.o)

all:	$(NAME)

$(NAME):	$(OBJ)
	$(CC) -o $(NAME) $(OBJ) -lncurses -DNCURSES_STATIC -fsanitize=leak -fsanitize=address

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re:	fclean all

runtests:
	make clean
	make
	./chessformer -s test_maps/capability1
	./chessformer -s test_maps/capability2
	./chessformer -s test_maps/capability3
	./chessformer -s test_maps/capability4
	./chessformer -s test_maps/capability5
	./chessformer -s test_maps/capability6
	./chessformer -s test_maps/capability7
	./chessformer -s test_maps/capability8
	./chessformer -s test_maps/capability9
	./chessformer -s test_maps/capability10
	./chessformer -s test_maps/capability11
	./chessformer -s test_maps/capability12
	./chessformer -s test_maps/incapability1
	./chessformer -s test_maps/incapability2

.PHONY: all clean fclean re
