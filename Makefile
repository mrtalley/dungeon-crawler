default: game_new

game_new: game_new.c
	gcc -o game_new game_new.c -Wall -Werror -ggdb

clean:
	rm -f game *~
