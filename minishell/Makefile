# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fsilva-p <fsilva-p@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/28 14:37:09 by fsilva-p          #+#    #+#              #
#    Updated: 2025/02/13 17:11:09 by fsilva-p         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

LIBFT_PATH = ./libs/libft
LIBFT = $(LIBFT_PATH)/libft.a

GREEN := \033[0;32m
PASTEL_GREEN := \033[38;5;120m
CYAN := \033[0;36m
NC := \033[0m
TICK = ✔

NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -g
READLINEFLAG = -lreadline

SRCS = ./srcs/minishell.c\
       ./srcs/print_banner.c\
       ./srcs/handle_quotes.c\
       ./srcs/get_env.c\
       ./tokens/token.c\
       ./srcs/ft_strtok.c\
       ./srcs/parse_input.c\
       ./srcs/redirections.c\
       ./srcs/execute_exter_cmds.c\
       ./builtins/cd.c \
       ./builtins/cd_utils.c \
			 ./builtins/cd_utils2.c \
       ./builtins/cmd_builtins_env_pwd.c \
			 ./builtins/cmd_builtins_echo.c \
       ./builtins/cmd_builtins_exit.c \
			 ./builtins/cmd_builtins_exit_utils.c \
       ./builtins/cmd_builtins_export.c \
       ./builtins/cmd_builtins_export_utils.c \
			 ./builtins/cmd_builtins_export_utilsb.c \
			 ./builtins/cmd_builtins_export_utils2.c \
			 ./builtins/cmd_builtins_unset_utils.c \
       ./builtins/cmd_builtins_unset.c \
       ./builtins/exec.c \
			 ./builtins/exec2.c \
			 ./builtins/exec3.c \
       ./builtins/exit.c \
			 ./builtins/exit_utils.c \
       ./builtins/utils.c\
			 ./builtins/utils2.c\
       ./srcs/pipes.c\
       ./srcs/apply_redir.c\
       ./srcs/split_dollarsign.c\
       ./srcs/free_argv.c\
       ./srcs/ft_getpid.c\
       ./srcs/handle_redirection.c\
       ./srcs/pipes_utils.c\
       ./srcs/call_functions.c\
       ./srcs/minishell_utils.c\
       ./tokens/tokens_utils.c
OBJS = $(SRCS:.c=.o)

COUNT = 0
TOTAL = $(words $(SRCS))

all: $(LIBFT) $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	@echo "\n\t$(CYAN)$(TICK) MINISHELL compiled 🥳😺✨ (miaou)$(NC)\n"
	@$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBFT) $(READLINEFLAG)

%.o: %.c
	$(eval COUNT=$(shell echo $$(($(COUNT) + 1))))
	@echo "$(PASTEL_GREEN)[$(COUNT)/$(TOTAL)] Compiling $<$(NC)"
	@$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
	@make -C $(LIBFT_PATH)

clean:
	@echo "$(GREEN)Cleaning...$(NC)"
	@rm -f $(OBJS)
	@make clean -C $(LIBFT_PATH)

fclean: clean
	@rm -f $(NAME)
	@make fclean -C $(LIBFT_PATH)

re: fclean all

.PHONY: all fclean clean re $(LIBFT)
