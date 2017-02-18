##
## Makefile for Makefile in /home/jojo/Documents/epitech/PSU_2016_navy
## 
## Made by Joseph Queuche
## Login   <joseph.queuche@epitech.eu>
## 
## Started on  Tue Jan 31 17:24:11 2017 Joseph Queuche
## Last update Sat Feb 18 18:38:53 2017 Joseph Queuche
##

SRC	=	angle.c		\
		main_test.c

OBJ	=	$(SRC:.c=.o)

NAME	=	angle

CC	=	gcc

LIBS	=	-lm

CFLAGS	+=	-g3

RM	=	rm -rf

all:		$(NAME)

$(NAME):	$(OBJ)
		$(CC) $(OBJ) -o $(NAME) $(LIBS)

clean:
		$(RM) $(OBJ)

fclean:		clean
		$(RM) $(NAME)

re:		fclean all

.PHONY: all clean fclean re
