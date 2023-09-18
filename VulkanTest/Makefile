CC = g++
CFLAGS = -std=c++17 -Iincludes/ -O2 -g -DNDEBUG
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

SRCS = $(wildcard srcs/*.cpp)
OBJS = $(patsubst srcs/%.cpp, obj/%.o, $(SRCS))

INCS = $(wildcard includes/*.hpp)

NAME = VulkanTest

OBJDIR = obj

COLOR_RESET = \033[0m
COLOR_COMPILE = \033[1;34m
COLOR_LINK = \033[1;32m
COLOR_REMOVE = \033[1;31m

$(OBJDIR)/%.o: srcs/%.cpp
	@mkdir -p $(dir $@)
	@printf '$(COLOR_COMPILE)Compiling$(COLOR_RESET) %s\n' $<
	@$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS) $(INCS)
	@printf '$(COLOR_LINK)Linking objs...$(COLOR_RESET)\n'
	@$(CC) $(OBJS) -o $(NAME) $(LDFLAGS)
	@printf '$(COLOR_LINK)Finished linking √$(COLOR_RESET)\n'

all: $(NAME)

debug: CFLAGS := $(filter-out -DNDEBUG,$(CFLAGS))
debug: all

clean:
	@printf '$(COLOR_REMOVE)Removing$(COLOR_RESET) %s\n' $(OBJDIR)
	@$(RM) -r $(OBJDIR)

fclean: clean
	@printf '$(COLOR_REMOVE)Removing$(COLOR_RESET) %s\n' $(NAME)
	@$(RM) $(NAME)

re: fclean all

.PHONY: all debug clean fclean re
