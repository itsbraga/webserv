#————————————————————————————————————————————————————————
#	ANSI
#————————————————————————————————————————————————————————

NC			:=	\e[0m
BOLD		:=	\e[1m
BLINK		:=	\e[5m

WHITE		:=	\e[97m
BLUE		:=	\e[34m
P_BLUE		:=	\e[38;2;179;235;242m
CYAN		:=	\e[36m
P_YELLOW	:=	\e[38;2;255;234;150m
GREEN		:=	\e[32m
G_NEON		:=	\e[38;2;57;255;20m
P_GREEN		:=	\e[38;2;173;235;179m
PURPLE		:=	\e[35m
P_PURPLE	:=	\e[38;2;211;211;255m
PINK		:=	\e[38;2;255;182;193m

#————————————————————————————————————————————————————————
#	PROGRAM NAME & COMPILATION DETAILS
#————————————————————————————————————————————————————————

NAME		:=	webserv
INC			:=	-I INCLUDES/

CPPFLAGS	:=	-Wall -Wextra -Werror -std=c++98
DEPFLAGS	:=	-MMD -MP
DEBUG		:=	-g3

define display_ascii_art
	@printf "%b\n" "$(1)                 ______                            \n  ___      _________  /_______________________   __ \n  __ | /| / /  _ \_  __ \_  ___/  _ \_  ___/_ | / /  \n  __ |/ |/ //  __/  /_/ /(__  )/  __/  /   __ |/ /  \n  ____/|__/ \___//_.___//____/ \___//_/    _____/"
endef

#————————————————————————————————————————————————————————
#	SOURCES
#————————————————————————————————————————————————————————

SRCS_DIR	:=	SRCS/
SRCS		:=	$(sort $(shell find SRCS -type f -name '*.cpp'))

OBJS_DIR	:=	OBJS/
OBJS		:=	$(patsubst $(SRCS_DIR)%.cpp,$(OBJS_DIR)%.o,$(SRCS))
DEPS		:=	$(OBJS:.o=.d)

#————————————————————————————————————————————————————————
#	PROGRESS BAR
#————————————————————————————————————————————————————————

TOTAL_FILES		:=	$(words $(SRCS))
PROGRESS_FILE	:=	.progress_count

BAR_WIDTH		:=	55
BAR_FILL		:=	█
BAR_EMPTY		:=	░

define show_progress
	@CURRENT=$$(cat $(PROGRESS_FILE) 2>/dev/null || echo 0); \
	CURRENT=$$((CURRENT + 1)); \
	echo $$CURRENT > $(PROGRESS_FILE); \
	TOTAL=$(TOTAL_FILES); \
	if [ $$TOTAL -gt 0 ]; then \
		PERCENT=$$((CURRENT * 100 / TOTAL)); \
		FILLED=$$((CURRENT * $(BAR_WIDTH) / TOTAL)); \
		EMPTY=$$(($(BAR_WIDTH) - FILLED)); \
		printf "\r\033[K$(BOLD)$(WHITE)["; \
		for i in $$(seq 1 $$FILLED); do printf "$(P_GREEN)$(BAR_FILL)"; done; \
		for i in $$(seq 1 $$EMPTY); do printf "$(WHITE)$(BAR_EMPTY)"; done; \
		printf "$(WHITE)] $(P_GREEN)%3d%%\t$(P_PURPLE)(%d/%d)$(NC) " $$PERCENT $$CURRENT $$TOTAL; \
		printf "$(P_PURPLE)$(1)$(NC)"; \
		if [ $$CURRENT -eq $$TOTAL ]; then printf "\n"; fi; \
	fi
endef

#————————————————————————————————————————————————————————
#	RULES
#————————————————————————————————————————————————————————

all: init_progress $(NAME)

init_progress:
		@rm -f $(PROGRESS_FILE)
		@echo "0" > $(PROGRESS_FILE)
		@printf "$(BOLD)╔══════════════════════════════════════════════════════╗$(NC)\n"
		@printf "$(BOLD)║$(NC)\t\t  $(BOLD)$(PINK)Compiling $(NAME)...$(NC)                 $(BOLD)║$(NC)\n"
		@printf "$(BOLD)╚══════════════════════════════════════════════════════╝$(NC)\n\n"


$(OBJS_DIR)%.o: $(SRCS_DIR)%.cpp
		@mkdir -p $(dir $@)
		@c++ $(CPPFLAGS) $(DEPFLAGS) $(INC) -c $< -o $@
		@$(call show_progress,$<)

-include $(DEPS)

$(NAME): $(OBJS)
		@c++ $(CPPFLAGS) $(INC) $(OBJS) -o $(NAME)
		@rm -f $(PROGRESS_FILE)
		@printf "\033[5A\033[J"
		@echo "\n$(BOLD)======================================================\n"
		@$(call display_ascii_art,$(P_GREEN))
		@echo "\n$(NC)$(BOLD)"
		@echo "=====================$(BLINK)$(G_NEON)   READY!   $(NC)$(BOLD)=====================\n\n"

clean:
		rm -rf $(OBJS_DIR)
		@rm -f $(PROGRESS_FILE)
		@echo "$(BOLD)$(BLUE)[clean]:\t$(NC)Objects successfully removed!\n"

fclean:	clean
		rm -rf $(NAME)
		@echo "$(BOLD)$(CYAN)[fclean]:\t$(NC)Executable successfully removed!\n"

re:	fclean
		@make --no-print-directory all
		@echo "$(BOLD)$(P_YELLOW)[webserv: destination_finale] $(NC)Project successfully rebuilt! ✨\n"

debug: fclean
		@make $(NAME) CPPFLAGS="$(CPPFLAGS) $(DEBUG)"


.PHONY: all init_progress clean fclean re debug