default: dungeon

dungeon: dungeon.c heap.c distance_map.c
	gcc -o dungeon dungeon.c heap.c distance_map.c -Wall -Werror -ggdb

clean:
	rm -f dungeon *~
