#Makefile para fazer o jogo SpaceWars

# ---- V A R I Á V E I S   D E   A M B I E N T E ---- #

VPATH = ./fisica:./graficos:./obj

CC = gcc
CFLAGS = -Wall -O2 -Wno-unused-result -lm

OBJ_PATH = ./obj

# ---- R E G R A S ---- #

default : debug move_o

debug : debug.o IO.o auxiliar.o vetores.o fisica.o
	${CC} $^ ${CFLAGS} -o $@

debug.o : debug.c IO.h
	${CC} -c $< ${CFLAGS} -o $@

IO.o : IO.c IO.h vetores.h fisica.h auxiliar.h
	${CC} -c $< ${CFLAGS} -o $@

auxiliar.o : auxiliar.c auxiliar.h
	${CC} -c $< ${CFLAGS} -o $@

vetores.o : vetores.c vetores.h
	${CC} -c $< ${CFLAGS} -o $@

fisica.o : fisica.c fisica.h vetores.h auxiliar.h
	${CC} -c $< ${CFLAGS} -o $@

move_o :
	mv -f *.o $(OBJ_PATH) 2>/dev/null; true

.PHONY: clean_o clean_debug clean_all

clean_all : clean_o clean_debug

clean_o :
	rm -f *.o
	rm -f $(OBJ_PATH)/*
	rmdir -f $(OBJ_PATH)

clean_debug :
	rm -f debug

