#————————————————————————————————————————————————————————
#	ANSI
#————————————————————————————————————————————————————————

NC		:=	\e[0m
BOLD	:=	\e[1m
BLINK	:=	\e[5m

BLUE	:=	\e[34m
CYAN	:=	\e[36m
YELLOW	:=	\e[38;2;255;234;150m
G_NEON	:=	\e[38;2;57;255;20m
P_GREEN	:=	\e[38;2;173;235;179m
PURPLE	:=	\033[38;2;211;211;255m

#————————————————————————————————————————————————————————
#	PROGRAM NAME & COMPILATION DETAILS
#————————————————————————————————————————————————————————

NAME		:=	webserv
INC			:=	-I INCLUDES/

CPPFLAGS	:=	-Wall -Wextra -Werror -std=c++98
DEPFLAGS	:=	-MMD -MP
DEBUG		:=	-g3

define display_compiled_file
	@printf "$(BOLD)$(ITAL)$(1)Compiled: $(NC)$(ITAL)$(2)\n$(NC)"
endef

define display_ascii_art
	@printf "%b\n" "$(1)                 ______                            \n  ___      _________  /_______________________   __ \n  __ | /| / /  _ \_  __ \_  ___/  _ \_  ___/_ | / /  \n  __ |/ |/ //  __/  /_/ /(__  )/  __/  /   __ |/ /  \n  ____/|__/ \___//_.___//____/ \___//_/    _____/"
endef

#————————————————————————————————————————————————————————
#	SOURCES
#————————————————————————————————————————————————————————

SRCS_DIR	:=	SRCS/
SRCS		:=	$(sort $(shell find SRCS -type f -name '*.cpp'))
#SRCS		:=	$(sort $(shell find SRCS -type f \( -name '*.cpp' -o -name '*.ipp' \)))

OBJS_DIR	:=	OBJS/
OBJS		:=	$(patsubst $(SRCS_DIR)%.cpp,$(OBJS_DIR)%.o,$(SRCS))
#OBJS		+=	$(patsubst $(SRCS_DIR)%.ipp,$(OBJS_DIR)%.o,$(SRCS))
DEPS		:=	$(OBJS:.o=.d)

#————————————————————————————————————————————————————————
#	RULES
#————————————————————————————————————————————————————————

all: $(NAME)

$(OBJS_DIR)%.o: $(SRCS_DIR)%.cpp
				@mkdir -p $(dir $@)
				@$(call display_compiled_file,$(PURPLE),$<)
				@c++ $(CPPFLAGS) $(DEPFLAGS) $(INC) -c $< -o $@
-include $(DEPS)

$(NAME): $(OBJS)
			@c++ $(CPPFLAGS) $(INC) $(OBJS) -o $(NAME)
			@echo "\n\n$(BOLD)======================================================\n"
			@$(call display_ascii_art,$(P_GREEN))
			@echo "\n$(NC)$(BOLD)"
			@echo "=====================$(BLINK)$(G_NEON)   READY!   $(NC)$(BOLD)=====================\n\n"

clean:
		rm -rf $(OBJS_DIR)
		@echo "$(BOLD)$(BLUE)[objects]:\t$(NC)Removed!\n"

fclean:	clean
			rm -rf $(NAME)
			@echo "$(BOLD)$(CYAN)[executable]:\t$(NC)Removed!\n\n"

re:	fclean all
		@echo "$(BOLD)$(YELLOW)[webserv] $(NC)Project successfully rebuilt! ✨\n"

debug: fclean
		@make $(NAME) CPPFLAGS="$(CPPFLAGS) $(DEBUG)"

.PHONY: all clean fclean re debug