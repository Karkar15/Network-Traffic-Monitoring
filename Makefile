CC = gcc
CFLAGS = -Wall -O2
LDFLAGS = -lm

all: cms cms_experiment

cms: src/cms.c
	$(CC) $(CFLAGS) -o cms src/cms.c $(LDFLAGS)

cms_experiment: src/cms_experiment.c
	$(CC) $(CFLAGS) -o cms_experiment src/cms_experiment.c $(LDFLAGS)

clean:
	rm -f cms cms_experiment

run: cms
	./cms < examples/input.txt

run-experiment: cms_experiment
	./cms_experiment < examples/experiment_input.txt