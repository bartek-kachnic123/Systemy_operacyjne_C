.SUFFIXES: .c .cpp .o .x .h
DIR = $(notdir $(CURDIR))
SYSNAME = $(shell uname -s)
# Nazwy:
NAME1 = powielacz
NAME2 = wzajemne_wykluczanie
NAME3 = semafor_biblio
#Dane do uruchomienia programu:
LICZBA_PROCESOW = 5
PLIK = numer.txt
# Program wykonawczy:
EXEC1  = $(NAME1).x
EXEC2 = $(NAME2).x
# Pliki powstale po kompilacji plikow zrodlowych, tzw. object files
OBJS1  = $(NAME1).o
OBJS2 = $(NAME2).o
#------------------------------------------------------------------------------
# dla biblioteki dzielonej
OBJS3  = $(NAME3).o 
LIB_NAME = _Semafor
# Katalog dla bibliotek
LIB_DIR = ./lib
# Biblioteka dzielona linkowana dynamicznie - nazwa zalezy od systemu:
# Linux - rozszerzenie: .so
# macOS - rozszerzenie: .dylib
# Dyrektywa warunkowa sprawdzajaca, czy system nazywa sie Linux
# i wykonujaca okreslone polecenie.
ifeq ($(SYSNAME), Linux)
  LIB_SHAR = lib$(LIB_NAME).so
endif
# macOS - nazwa jadra to Darwin
ifeq ($(SYSNAME), Darwin)
  LIB_SHAR = lib$(LIB_NAME).dylib
endif
# Pliki skompilowane wchodzace do biblioteki
LIB_OBJS = $(OBJS3)
# Pliki naglowkowe
HEADS3 =  $(NAME3).h
#------------------------------------------------------------------------------
# Opcje kompilatora i linkera; -fPIC potrzebna dla bibliotek dzielonych
CFLAGS = -Wall -std=c99 -pedantic -O -fPIC 
LFLAGS = -Wall -std=c99 -pedantic -O 
# Biblioteki linkera zainstalowane w systemie 
# (tu: biblioteka semaforow)
PDLIBS = -pthread
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

###############################################################################

###############################################################################
# Tworzenie biblioteki dzielonej (shared library)
$(LIB_SHAR): $(LIB_OBJS)
# Linux - opcja linkera: -shared
ifeq ($(SYSNAME), Linux)
	$(LD) -shared -o $@ $(LFLAGS) $(LIB_OBJS)
endif
# macOS - opcje linkera: -dynamiclib,  -install_name @rpath/$(LIB_DIR)/$@
ifeq ($(SYSNAME), Darwin)
	$(LD) -dynamiclib -o $@ $(LFLAGS) $(LIB_OBJS) -install_name @rpath/$(LIB_DIR)/$@
endif
	mkdir -p $(LIB_DIR)
	mv $(LIB_SHAR) $(LIB_DIR)
###############################################################################
#-------------------------------------------------------------------------------------------------
#  Plik wykonawczy z biblioteka dzielona - wersja 2
#  -> informacja o lokalizacji biblioteki nie jest przekazywana przez opcje 
#     linkera, lecz na etapie wykonania przez zmienna srodowiskowa:
#     LD_LIBRARY_PATH (patrz ponizej na przyklad uruchomienia programu).
$(EXEC1): $(OBJS1) $(LIB_SHAR)
	$(LD) -o $@ $(LFLAGS) $(OBJS1) -L$(LIB_DIR) -l$(LIB_NAME) $(PDLIBS)
$(EXEC2): $(OBJS2) $(LIB_SHAR)
	$(LD) -o $@ $(LFLAGS) $(OBJS2) -L$(LIB_DIR) -l$(LIB_NAME) $(PDLIBS)
###############################################################################
# Dodatkowe reguly zaleznosci plikow objectowych od naglowkowych, ktorych
# nie uwglednia regula wzorcowa (powyzej).
###############################################################################
$(OBJS1): $(HEADS3)
$(OBJS2): $(HEADS3)
###############################################################################
.PHONY: run1
###############################################################################  
run1: $(EXEC1) $(EXEC2)
# Linux
ifeq ($(SYSNAME), Linux)
	export LD_LIBRARY_PATH=${LIB_DIR}:${LD_LIBRARY_PATH}; \
	./$(EXEC1) $(EXEC2) $(LICZBA_PROCESOW) $(PLIK)
endif
# macOS - tu odpowiednia zmienna srodowiskowa to: DYLD_LIBRARY_PATH
ifeq ($(SYSNAME), Darwin)
	export DYLD_LIBRARY_PATH=${LIB_DIR}:${LD_LIBRARY_PATH}; \
	./$(EXEC1) $(EXEC2) $(LICZBA_PROCESOW) $(PLIK)
endif
###############################################################################
# Sprzataczka (tez "phony target")
###############################################################################
.PHONY: clean tar
clean:                                                     
	rm -f *.o  *~ *.a *.so *.x core core* a.out; rm -rf ${LIB_DIR}
# Archiwizacja i kompresja
tar: clean
	(cd ../; tar -cvzf $(DIR).tar.gz  $(DIR) )
