NAME      = philo_bonus

SRCS      = main.c tools.c thread.c check.c routine.c exit.c
SRC_DIR   = srcs/
OBJ_DIR   = objs/

OBJS      = $(addprefix $(OBJ_DIR), ${SRCS:.c=.o})
DIR		  = $(sort $(dir $(OBJS)))

DEP       = ${OBJS:.o=.d}

CC        = clang

CFLAGS    = -Iincludes -g -Wall -Wextra -Werror -pthread -MMD

RM        = rm -rf

MAKE	+= --no-print-directory

all :     
	@${MAKE} -j ${NAME}

$(NAME) :   ${OBJS}	
			@${CC} -o $@ ${OBJS} ${CFLAGS}
			@printf "\033[32mAll done.\nPlease enter : # number of philosophers # time to die # time to eat # time to sleep # number of time each have to eat ! :)\033[0m\n"

$(DIR) :    
	@mkdir -p $@

$(OBJ_DIR)%.o: $(SRC_DIR)%.c | $(DIR)
		$(CC) -c $< -o $@ ${CFLAGS}

clean :   
			@echo "Cleaning files..."
			@${RM} ${OBJ_DIR}
			@printf "\033[32mClean done !\033[0m\n"

fclean :	clean
			@echo "Cleaning the rest..."
	       @${RM} ${NAME}
			@printf "\033[32mFClean done !\033[0m\n"

re :	fclean 
		@${MAKE} all

-include  ${OBJS:.o=.d}

.PHONY :  all clean fclean re
