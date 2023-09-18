define compiling
	@mkdir -p $(dir $2)
	@printf '$(shell tput setaf 69)Compiling$(shell tput sgr0) %s\n' $1
	@$(CC) $(CFLAGS) -c $1 -o $2
endef

define removing
	@printf '$(shell tput setaf 59)Removing$(shell tput sgr0) %s\n' $1
	@$(RM) $1 > /dev/null
endef

SRCS =	srcs/device_and_queues.cpp \
		srcs/routine.cpp \
		srcs/validation_layers.cpp \
		srcs/swapchain.cpp \
		srcs/image_views.cpp \
		srcs/graphics_pipeline.cpp \
		srcs/render.cpp \
		srcs/main.cpp

OBJDIR   := obj
OBJS = $(SRCS:%.cpp=$(OBJDIR)/%.o)

INCS =	includes/hellotriangle.hpp \
		includes/utils.hpp

NAME = VulkanTest

RM = rm -f

CC = g++

CFLAGS = -std=c++17 -Iincludes/ -O2 -g

LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

$(OBJDIR)/%.o : %.cpp
	$(call compiling,$<,$(OBJDIR)/$*.o)

${NAME}: $(OBJS) $(INCS)
	@printf '$(shell tput setaf 29)Finished Compiling √ %s\n$(shell tput sgr0)' $1
	@printf '$(shell tput setaf 69)Linking objs...\n$(shell tput sgr0)'
	$(CC) $(OBJS) -o $(NAME) $(LDFLAGS)
	@printf '$(shell tput setaf 29)Finished linking √ %s\n$(shell tput sgr0)' $1

all: $(NAME)

clean:
	$(call removing,$(OBJS))

fclean: clean
	$(RM) $(NAME)
	$(RM) -r $(OBJDIR)

re: fclean all

.PHONY: all re clean fclean
