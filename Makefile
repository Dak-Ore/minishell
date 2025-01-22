NAME = minishell

GREEN = \033[32m
RED = \033[31m
YELLOW = \033[0;33m
RESET = \033[0m

LIBFT = libft/libft.a
INC = -I/opt/homebrew/opt/readline/include -I.
LIB = -L/opt/homebrew/opt/readline/lib -lreadline -L./libft/ -lft

FLAGS = -Wall -Wextra -Werror -g

SOURCES = \
        main.c \
		prompt.c \
		token.c \
		token_utils.c \
		utils.c \
		pathfinder.c \
		signal.c \
		builtin.c \
		builtin_env.c \
		builtin_utils.c \
		exec_builder.c \
		secure.c \
		execute.c \
		heredoc.c \
		env.c \
		expand.c \
		errors.c

OBJDIR = obj
OBJS = $(addprefix $(OBJDIR)/, $(SOURCES:.c=.o))

$(NAME): $(LIBFT) $(OBJS)
	@echo "$(GREEN)Compiling $(NAME)...$(RESET)"
	@cc $(FLAGS) $(OBJS) -o $(NAME) $(INC) $(LIB)
	@echo "$(GREEN)Compilation finished successfully!$(RESET)"

$(LIBFT):
	@echo "$(GREEN)Building Libft...$(RESET)"
	@make -s -C libft libft.a

all: $(NAME)

$(OBJDIR):
	@mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: srcs/%.c | $(OBJDIR)
	@cc $(FLAGS) $(INC) -c $< -o $@

norm:
	@echo "Lancement de la norminette..."
	@norminette | grep -i "error" > norm_errors.log || true
	@if [ -s norm_errors.log ]; then \
		err_count=$$(wc -l < norm_errors.log); \
		echo "$(RED)$$err_count erreur(s) détectée(s) :$(RESET)"; \
		cat norm_errors.log; \
	else \
		echo "$(GREEN)OK - Aucun problème détecté par la norminette.$(RESET)"; \
	fi
	@rm norm_errors.log

clean:
	@echo "$(RED)Cleaning object files...$(RESET)"
	@rm -rf $(OBJDIR)
	@make -s -C libft clean

fclean: clean
	@echo "$(RED)Cleaning all...$(RESET)"
	@rm -f $(NAME)
	@make -s -C libft fclean

re: fclean $(NAME)

.PHONY: all clean fclean re
