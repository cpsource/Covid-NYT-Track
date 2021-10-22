
all: filter

filter: filter.c
	gcc -O2 -g -o filter filter.c

install-once:
	git clone https://github.com/nytimes/covid-19-data covid-19-data

clean:
	rm -f *.csv
	rm -f filter
