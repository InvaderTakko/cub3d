# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sruff <sruff@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/05 15:18:38 by sruff             #+#              #
#    Updated: 2025/07/05 15:44:09 by sruff            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


# gotta go fast: -ffast-math -Ofast -mtune=native -march=native -funroll-loops
NAME := cub3d
CFLAGS := -Wall -Wextra -Werror -g
LDFLAGS +=  -fsanitize=address -fsanitize=undefined 
LIBMLX := ./MLX42
LIBMLX_URL    := https://github.com/codam-coding-college/MLX42.git
LIBMLX_COMMIT := ce254c3a19af8176787601a2ac3490100a5c4c61
HEADERS := -I ./include -I $(LIBMLX)/include

LIBFT_DIR := ./libft
LIBFT     := $(LIBFT_DIR)/libft.a

# Directories
SRC_DIR := src
OBJ_DIR := obj

# OS specific flags
ifeq ($(shell uname), Darwin)
    BREW_PREFIX := $(shell brew --prefix)
    LDFLAGS += -L$(BREW_PREFIX)/lib
    LIBS := $(LIBMLX)/build/libmlx42.a -lglfw -lm -L$(LIBFT_DIR) -lft
else
    LIBS := $(LIBMLX)/build/libmlx42.a -ldl -lglfw -pthread -lm -L$(LIBFT_DIR) -lft
endif

SRCS := \
  src/main.c \
  src/render.c \
  src/render_draw.c \
  src/render_dda.c \
  src/render_texture.c \
  src/render_walls.c \
  src/player.c \
  src/utils.c \
  src/utils1.c \
  src/check_walls.c \
  src/gc_malloc.c \
  src/gc_utils.c \
  src/gc_split.c \
  src/init_player.c \
  src/mlx_load_texture.c \
  src/mlx_setup.c \
  src/keypress.c \
  src/pad_map_grid.c \
  src/parsing.c \
  src/parse_colors.c \
  src/parse_textures.c \
  src/parse_file.c \
  src/validate_map.c
OBJS := $(patsubst src/%.c,obj/%.o,$(SRCS))

all: $(LIBFT) libmlx $(NAME)
	@echo "$(NAME) successfully built!"

libmlx:
	@if [ ! -d "$(LIBMLX)" ]; then \
			git clone $(LIBMLX_URL) $(LIBMLX); \
			cd $(LIBMLX) && git checkout $(LIBMLX_COMMIT); \
	fi
	@cmake -S $(LIBMLX) -B $(LIBMLX)/build
	@cmake --build $(LIBMLX)/build --parallel 4

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -o $@ -c $< $(HEADERS) && printf "Compiling: $(notdir $<)\n"

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(NAME): $(OBJS)
	@$(CC) $(OBJS) $(LIBS) $(HEADERS) $(LDFLAGS) -o $(NAME)

clean:
	@rm -rf $(OBJ_DIR)
	@rm -rf $(LIBMLX)/build
	@$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	@rm -rf $(NAME) $(BONUS_NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

mlxfclean: fclean
	@rm -rf $(LIBMLX)

.PHONY: all clean fclean re libmlx
