# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: qliso <qliso@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/15 10:15:28 by qliso             #+#    #+#              #
#    Updated: 2025/03/28 14:02:04 by qliso            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS=$(addprefix ./srcs/, \
		$(addsuffix .c, \
			00_main \
			10_init \
			11_init \
			20_parsing \
			21_parsing \
			22_parsing \
			23_parsing \
			24_parsing \
			25_parsing \
			26_parsing \
			30_player \
			31_player \
			32_player \
			33_player \
			40_textures \
			50_rendering \
			51_rendering \
			52_rendering \
			53_rendering \
			54_rendering \
			55_rendering \
			56_rendering \
			60_freeing \
			61_freeing \
			70_utils \
			80_err \
		)\
	)
OBJ=$(SRCS:%.c=%.o)
HEADER=./srcs/cub3d.h
NAME=cub3d

LIB=libft.a
LIB_FOLD=./libft/
MLX=libmlx.a
MLX_FOLD=./minilibx-linux/

CCFLAGS=-Wall -Werror -Wextra -g
IFLAGS=-I./minilibx-linux -I./libft/includes
LFLAGS=-L./minilibx-linux -L./libft -lmlx -lXext -lX11 -lft -lm

.PHONY: all clean fclean re

YELLOW=\033[1;33m
GREEN=\033[1;32m
WHITE=\033[0;37m

#-------------------------------------------

all 	: $(LIB) $(MLX) $(NAME) 

ess		: $(LIB) $(MLX)
		  $(CC) $(CCFLAGS) $(IFLAGS) srcs/ess.c -o ess $(LFLAGS)

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