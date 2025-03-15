# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: qliso <qliso@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/15 10:15:28 by qliso             #+#    #+#              #
#    Updated: 2025/03/15 16:05:35 by qliso            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS=$(addprefix ./srcs/, \
		$(addsuffix .c, \
			main \
			utils \
		)\
	)
OBJ=$(SRCS:%.c=%.o)
HEADER=./srcs/cub3d.h
NAME=cub3d

LIB=libft.a
LIB_FOLD=./libft/
MLX=libmlx.a
MLX_FOLD=./minilibx-linux/

CCFLAGS=-Wall -Werror -Wextra
IFLAGS=-I./minilibx-linux -I./libft/includes
LFLAGS=-L./minilibx-linux -L./libft -lmlx -lXext -lX11 -lft -lm

.PHONY: all clean fclean re

YELLOW=\033[1;33m
GREEN=\033[1;32m
WHITE=\033[0;37m

#-------------------------------------------

all 	: $(LIB) $(MLX) $(NAME) 


$(LIB)	: $(LIB_FOLD)$(LIB)

$(LIB_FOLD)$(LIB)	:
		@echo "$(YELLOW)🦒🦒🦒 COMPILING LIBFT 🦒🦒🦒$(WHITE)"
		make -C $(LIB_FOLD) all
		@echo "$(GREEN)Libft Compiled !\n$(WHITE)"

$(MLX)	: $(MLX_FOLD)$(MLX)

$(MLX_FOLD)$(MLX)	:
		@echo "$(YELLOW)🦒🦒🦒 COMPILING MLX 🦒🦒🦒$(WHITE)"
		@make -C $(MLX_FOLD)
		@echo "$(GREEN)Mlx Compiled !\n$(WHITE)"


$(NAME)	: $(OBJ)
		@echo "$(YELLOW)🦒🦒🦒 COMPILING CUB3D 🦒🦒🦒$(WHITE)"
		$(CC) $(CCFLAGS) $(IFLAGS) $(OBJ) -o $(NAME) $(LFLAGS)
		@echo "$(GREEN)CUB3D Compiled !\n$(WHITE)"

%.o		: %.c $(HEADER)
		@$(CC) $(CCFLAGS) $(IFLAGS) -c $< -o $@

clean	:
		@echo "$(YELLOW)🦒🦒🦒 CLEANING CUB3D 🦒🦒🦒$(WHITE)"
		@rm -rf $(OBJ)
		@make -C $(LIB_FOLD) clean
		@make -C $(MLX_FOLD) clean
		@echo "$(GREEN)Clean completed !\n$(WHITE)"

fclean	: clean
	@echo "$(YELLOW)🦒🦒🦒 FCLEANING CUB3D 🦒🦒🦒$(WHITE)"
		@rm -rf $(NAME)
		@rm -rf $(LIB_FOLD)$(LIB)
		@rm -rf $(MLX_FOLD)$(MLX)
		@echo "$(GREEN)Fclean completed !\n$(WHITE)"

re		: fclean all