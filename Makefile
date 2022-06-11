NAME = philosophers
CC = gcc
CFLAGS = -Wall -Wextra -Werror  -pthread

SRC = main.c
OBJ = $(SRC:.c=.o)

${NAME}: $(OBJ)
	$(CC) $(OBJ) -o ${NAME} 

all: ${NAME}

bonus:
	echo "bonus"


clean: 
	rm -f ${OBJ}

fclean: clean
	rm -f ${NAME}

re: clean all

.PHONY: all clean fclean bonus