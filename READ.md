This respository uses the NYTIMES covid database
to track your covid numbers.

To Install
	make install-once
	make

To Run Daily
	./daily-run.sh

This produces Astates.csv, and tracks all states
that are listed in astates.txt.

Astates.csv format

	There are four colums that are

	1. The date
	2. The population
	3. The number of cases
	4. The number of deaths


Note
	The daily-run.sh script also produces a
county look for Orleans county in Vermont. You can modify
this script to dump information on any particular county
you are interested in.

Note
	You will note that daily-run.sh calls do.sh. This
script in turn calls filter of the form

	./filter -X

	Where X is a letter from a to z. You will see
that ./filter -a gets run. This tells filter to look in
file Astates.txt for the list of states and will
produce file astates.csv.

	To look at your own state, do this

	cp Astates.txt Xstates.txt
	Now edit Xstates.txt for the states you want
	Add the command to do.sh
	After daily-run.sh, you will produce xstates.csv


Note
	Instead of bothering with any of this, you can just google covid
numbers by state or county. But the advantage here is that you can
load the csv file into a spread sheet and do your own projections or
run statistical analysis that may not be available..



