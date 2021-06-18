all: main

main.o: main.c
	gcc -c main.c -g
	
list.o: list.c
	gcc -c list.c -g
	
htable.o: htable.c
	gcc -c htable.c -g
	
http_get_json_data.o: http_get_json_data.c
	gcc -c http_get_json_data.c -g
	
solar.o: solar.c
	gcc -c solar.c -g

main: main.o list.o htable.o http_get_json_data.o solar.o
	gcc main.o list.o htable.o http_get_json_data.o solar.o -lcurl -ljansson -o main

	
clean:
	rm *.o main
