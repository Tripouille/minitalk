BINS		= server client
SERVER_SRCS	= $(addprefix srcs/, server.c utils.c)
CLIENT_SRCS	= $(addprefix srcs/, client.c utils.c)
SERVER_OBJS	= $(SERVER_SRCS:srcs/%.c=objs/%.o)
CLIENT_OBJS	= $(CLIENT_SRCS:srcs/%.c=objs/%.o)
DEPS		= $(SERVER_SRCS:srcs/%.c=deps/%.d) $(CLIENT_SRCS:srcs/%.c=deps/%.d)

CC		= gcc
CFLAGS	= -Wall -Werror -Wextra -g3 -Iincludes
DFLAGS	= -MT $@ -MMD -MP -MF deps/$*.d

all: $(BINS)

objs/%.o: srcs/%.c
	$(CC) $(CFLAGS) $(DFLAGS) -c $< -o $@

server: $(SERVER_OBJS)
	$(CC) $(CFLAGS) $^ -o $@

client: $(CLIENT_OBJS)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -rf $(SERVER_OBJS) $(CLIENT_OBJS) $(DEPS) *.dSYM

fclean: clean
	rm -rf $(BINS)

re: fclean all

.PHONY: all clean fclean re
-include $(DEPS)
