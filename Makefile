SRC := dynamic_array.c

example: example.c $(SRC)
	gcc -o example example.c $(SRC)

debug: example.c $(SRC)
	gcc -g -o example example.c $(SRC)

clean:
	rm example
