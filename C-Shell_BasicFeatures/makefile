CC = gcc
CFLAGS = -Wall -g
OBJ = main.o command_cd.o command_echo.o command_pwd.o command_history.o utilities.o

my_shell: $(OBJ)
	$(CC) -o my_shell $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o my_shell
