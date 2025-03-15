# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nefadli <nefadli@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/07 15:11:52 by nefadli           #+#    #+#              #
#    Updated: 2025/03/07 16:41:24 by nefadli          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


GREEN = \033[32m
RED = \033[31m
YELLOW = \033[33m
RESET = \033[0m
CYAN = \033[36m

LINKING = 🚀
COMPILING = 🔨
CLEANING = 🧹
REMOVING = ❌
SUCCESS = ✅


CC = gcc
CFLAGS = -Wall -Wextra -Werror -I. -I./minilibx-linux -I./ft_getnextline
MLX_DIR = minilibx-linux
MLX = $(MLX_DIR)/libmlx.a 
SRC_DIR = src
OBJ_DIR = obj
SRC = $(wildcard $(SRC_DIR)/*.c) $(wildcard ft_getnextline/*.c)
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

NAME = cub3d


run: all
	@echo "$(CYAN)==============================$(RESET)"
	@echo "$(GREEN)Running the program...$(RESET)"
	./$(NAME)


all: $(NAME)


$(NAME): $(OBJ)
	@echo "$(CYAN)==============================$(RESET)"
	@echo "$(GREEN)$(LINKING) Linking...$(RESET)"
	$(CC) $(OBJ) -o $(NAME) $(MLX) -lm 
	@echo "$(GREEN)$(SUCCESS) Compilation finished successfully!$(RESET)"


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	@echo "$(YELLOW)$(COMPILING) Compiling $<...$(RESET)"
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "$(GREEN)$(SUCCESS) Compiled $< successfully!$(RESET)"

clean:
	@echo "$(RED)$(CLEANING) Cleaning up...$(RESET)"
	rm -rf $(OBJ_DIR)

fclean: clean
	@echo "$(RED)$(REMOVING) Removing executable...$(RESET)"
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re run