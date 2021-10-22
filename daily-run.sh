#!/bin/bash
#! Run this script daily
cd covid-19-data
git pull --ff-only
cat us-counties.csv | grep Orleans | grep Vermont >../Vermont-Orleans.csv
cd ..
./do.sh
tail Vermont-Orleans.csv

