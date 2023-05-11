SRCDIR = server/

SRCS = $(wildcard $(SRCDIR)*.c)

OBJS = ${SRCS:.c=.o}

NAME = knuTuimer

CFLAGS = -Iincludes/. -g3 -Wall -Werror
LDFLAGS = 

CC = gcc
RM = rm -f

${NAME}:	${OBJS}
		${CC} -o ${NAME} ${OBJS} ${LDFLAGS}

all:		${NAME}

clean:
		${RM} ${OBJS} *~ #*#

fclean:		clean
		${RM} ${NAME}

re:		fclean all
