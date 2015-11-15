all: rshell

FLG=-g -Wall -Werror -ansi -pedantic -std=c++11
CPLR=g++
DIR=bin

rshell: makebin src/main.cpp
	$(CPLR) $(FLG) src/main.cpp -o bin/rshell

makebin:
	@if [ ! -d $(DIR) ];\
	then \
	mkdir $(DIR); \
	fi