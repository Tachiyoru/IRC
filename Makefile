# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sleon <sleon@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/24 13:02:18 by adegain           #+#    #+#              #
#    Updated: 2023/09/25 14:54:01 by sleon            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME:=	ircserv

SRCS:=	main Server Client Channel Utils RFC1459 LIST INVITE NOTICE PRIVMSG KICK JOIN KILL MODE NAMES NICK OPER PASS PING QUIT TIME TOPIC USER PART
# --- Compilation ----
CC:= c++
SRCS_PATH:= srcs/
HDR_PATH:= inc/
OBJ_PATH:= obj/
CPP_FLAGS:= -Wall -Werror -Wextra -g3 -std=c++98 -I ${HDR_PATH}

SRC:= $(addprefix $(SRCS_PATH),$(addsuffix .cpp, $(SRCS)))
OBJ:= $(addprefix $(OBJ_PATH),$(addsuffix .o, $(SRCS)))
# ====================

# ------ Colors ------
BLUE:= "\033[38;5;117m"
LILAC:= "\033[38;5;147m"
PURPLE:= "\033[38;5;177m"
PINK:= "\033[38;5;207m"
MAGENTA:= "\033[38;5;197m"
ORANGE:= "\033[38;5;209m"
GREEN:= "\033[1;32m"
EOC:= "\033[0m"
# ====================

all:	${NAME}

${NAME}:		${OBJ}
				@echo -n ${LILAC} "C"
				@sleep 1e-1
				@echo -n ${LILAC} "o"
				@sleep 1e-1
				@echo -n ${LILAC} "m"
				@sleep 1e-1
				@echo -n ${LILAC} "p"
				@sleep 1e-1
				@echo -n ${LILAC} "i"
				@sleep 1e-1
				@echo -n ${LILAC} "l"
				@sleep 1e-1
				@echo -n ${LILAC} "i"
				@sleep 1e-1
				@echo -n ${LILAC} "n"
				@sleep 1e-1
				@echo -n ${LILAC} "g"
				@sleep 1e-1
				@${CC} ${CPP_FLAGS} ${SRC} -o ${NAME}
				@echo ${GREEN} " - OK" ${EOC}

${OBJ_PATH}%.o: ${SRCS_PATH}%.cpp
				@mkdir -p ${OBJ_PATH}
				@${CC} ${CPP_FLAGS} -c $< -o $@

%.cpp:
				@echo ${ORANGE} "Missing file: $@" ${EOC}

clean:
				@rm -rf ${OBJ_PATH}

fclean:			clean
				@rm -f ${NAME}
				@rm -rf ${NAME}.dSYM/
				@echo ${GREEN} " - CLEAN" ${EOC}

re:				fclean
				@${MAKE} all

.PHONY:			all clean fclean re
