#Makefile para fazer o jogo SpaceWars

# ---- V A R I Á V E I S   D E   A M B I E N T E ---- #

VPATH = ./base:./fisica:./graficos:./obj:./IO

CC = gcc
CFLAGS = -Wall -g -Wno-unused-result --pedantic -lm -lX11 -lXpm

OBJ_PATH = ./obj

# ---- R E G R A S ---- #

default : debug move_o

debug : debug.o IO.o auxiliar.o vetores.o fisica.o leitor.o gerenciadorBooster.o graficos.o xwc.o display.o
	${CC} $^ ${CFLAGS} -o $@

debug.o : debug.c IO.h
	${CC} -c $< ${CFLAGS} -o $@

IO.o : IO.c IO.h vetores.h fisica.h auxiliar.h leitor.h gerenciadorBooster.h
	${CC} -c $< ${CFLAGS} -o $@

auxiliar.o : auxiliar.c auxiliar.h
	${CC} -c $< ${CFLAGS} -o $@

vetores.o : vetores.c vetores.h
	${CC} -c $< ${CFLAGS} -o $@

fisica.o : fisica.c fisica.h vetores.h auxiliar.h
	${CC} -c $< ${CFLAGS} -o $@

leitor.o : leitor.c leitor.h auxiliar.h
	${CC} -c $< ${CFLAGS} -o $@

graficos.o : graficos.c graficos.h vetores.h auxiliar.h xwc.h
	${CC} -c $< ${CFLAGS} -o $@

xwc.o : xwc.c xwc.h
	${CC} -c $< ${CFLAGS} -o $@

display.o : display.c display.h graficos.h vetores.h fisica.h auxiliar.h
	${CC} -c $< ${CFLAGS} -o $@

gerenciadorBooster.o : gerenciadorBooster.c gerenciadorBooster.h fisica.h auxiliar.h vetores.h
	${CC} -c $< ${CFLAGS} -o $@

move_o :
	mkdir $(OBJ_PATH) 2>/dev/null; mv -f *.o $(OBJ_PATH) 2>/dev/null; true

.PHONY: clean_o clean_debug clean_all

clean_all : clean_o clean_debug

clean_o :
	rm -f *.o
	rm -f $(OBJ_PATH)/*
	rmdir $(OBJ_PATH)

clean_debug :
	rm -f debug
