CC = gcc
CFLAGS = -Wall

cms: src/cms.c
	$(CC) $(CFLAGS) -o cms src/cms.c -lm

clean:
	rm -f cms