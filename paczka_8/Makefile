.SUFFIXES: .c .cpp .o .x .h
DIR = $(notdir $(CURDIR))
SYSNAME = $(shell uname -s)
# Nazwy:
NAME1 = server
NAME2 = client

NAME4 = mqueue_biblio

#Dane do uruchomienia programu:

# Programy wykonawcze 
EXEC1 = $(NAME1).x
EXEC2  = $(NAME2).x
# Pliki powstale po kompilacji plikow zrodlowych, tzw. object files
OBJS1  = $(NAME1).o
OBJS2 = $(NAME2).o
#------------------------------------------------------------------------------
# dla biblioteki dzielonej
OBJS4 = $(NAME4).o
# Pliki skompilowane wchodzace do biblioteki
LIB_OBJS = $(OBJS4)
# Pliki naglowkowe
HEADS4 = $(NAME4).h

LIB_NAME = MQ
# Katalog dla bibliotek
LIB_DIR = ./lib
# Biblioteka statyczna (static library)
LIB_STAT = lib$(LIB_NAME).a
#------------------------------------------------------------------------------
# Opcje kompilatora i linkera; -fPIC potrzebna dla bibliotek dzielonych; -D_XOPEN_SOURCE=500 dla ftruncate()
CFLAGS = -Wall -std=c99 -pedantic -O -fPIC -D_XOPEN_SOURCE=500
LFLAGS = -Wall -std=c99 -pedantic -O -D_XOPEN_SOURCE=500
# Biblioteki linkera zainstalowane w systemie 
# (tu: biblioteka semaforow)
PDLIBS = -pthread
# (tu: dolaczenie biblioteki librt dla kolejek)
MQLIBS = -lrt
# Kompilator i linker (czesto ta sama nazwa)
CO = gcc
LD = $(CO)
# Program-archiwizer do tworzenia bibliotek statycznych
AR = ar
# Opcje archiwizera: tworzenie/uaktualnianie biblioteki wraz z indeksem plikow
#                    i wypisywanie komunikatow 
ARFLAGS = rsv
###############################################################################
# Ponizej przyklady definiowania regul zaleznosci
###############################################################################
%.o: %.c %.h
	$(CO) $(CFLAGS) -c $<
# Jak wyzej, ale bez zaleznosci od plikow naglowkowych
%.o: %.c
	$(CO) $(CFLAGS) -c $<

#------------------------------------------------------------------------------
# Tworzenie bibliteki statycznej
$(LIB_STAT): $(LIB_OBJS)
	$(AR) $(ARFLAGS) $@ $?
	mkdir -p $(LIB_DIR)
	mv $(LIB_STAT) $(LIB_DIR)
#------------------------------------------------------------------------------

$(EXEC1): $(OBJS1) $(LIB_STAT)
	$(LD) -o $@ $(LFLAGS) $(OBJS1) -L$(LIB_DIR) -l$(LIB_NAME) $(MQLIBS)
$(EXEC2): $(OBJS2) $(LIB_STAT)
	$(LD) -o $@ $(LFLAGS) $(OBJS2) -L$(LIB_DIR) -l$(LIB_NAME) $(MQLIBS)

$(OBJS1): $(HEADS4)
$(OBJS2): $(HEADS4)

# Uruchamianie
.PHONY: run1 run2
run1: $(EXEC1) $(EXEC2)
	xterm -hold -title $(NAME1) -bg red -e ./$(EXEC1) &
	xterm -hold -title $(NAME2)1 -bg green -e ./$(EXEC2) &
	xterm -hold -title $(NAME2)2 -bg green -e ./$(EXEC2) &


###############################################################################
# Sprzataczka (tez "phony target")
###############################################################################
.PHONY: clean tar
clean:                                                     
	rm -f *.o  *~ *.a *.so *.x core core* a.out; rm -rf ${LIB_DIR}
# Archiwizacja i kompresja
tar: clean
	(cd ../; tar -cvzf $(DIR).tar.gz  $(DIR) )
