#Makefile para fazer o jogo SpaceWars
#Paiolla coloca os comentários certinho aqui pq eu n sei fazer isso n

# ---- V A R I Á V E I S   D E   A M B I E N T E ---- #

VPATH = ./fisica:./objfiles

CC = gcc
CFLAGS = -Wall -O2 -Wno-unused-result -lm

# ---- R E G R A S ---- #

default : debug move_o

debug : debug.o IO.o auxiliar.o vetores.o fisica.o
	${CC} $^ ${CFLAGS} -o $@

debug.o : debug.c IO.h
	${CC} ${CFLAGS} -c $<

IO.o : IO.c IO.h vetores.h fisica.h auxiliar.h
	${CC} ${CFLAGS} -c $<

auxiliar.o : auxiliar.c auxiliar.h
	${CC} ${CFLAGS} -c $<

vetores.o : vetores.c vetores.h
	${CC} ${CFLAGS} -c $<

fisica.o : fisica.c fisica.h vetores.h auxiliar.h
	${CC} ${CFLAGS} -c $<

.PHONY: clean_o clean_debug clean_all move_o

move_o :
	touch dummy.o
	mkdir -p objfiles
	mv -f *.o objfiles
	rm -f ./objfiles/dummy.o

clean_all : clean_o clean_debug

clean_o :
	rm -f *.o
	rm -f ./objfiles/*.o

clean_debug :
	rm -f debug

