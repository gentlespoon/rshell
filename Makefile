all: rshell

FLG=-Wall -Werror -ansi -pedantic
CPLR=g++
DIR=bin

rshell: makebin src/main.cpp
	$(CPLR) $(FLG) src/main.cpp -o bin/rshell

makebin:
	@if [ ! -d $(DIR) ];\
	then \
	mkdir $(DIR); \
	fi