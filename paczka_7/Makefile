.SUFFIXES: .c .cpp .o .x .h
DIR = $(notdir $(CURDIR))
SYSNAME = $(shell uname -s)
# Nazwy:
NAME1 = main
NAME2 = producent
NAME3 = konsument

NAME5 = sem_biblio
NAME6 = shm_biblio
#Dane do uruchomienia programu:
DANE = dane.txt
WYNIKI = wyniki.txt
# Programy wykonawcze dla biblioteki statycznej:
EXEC1S  = $(NAME1)_s.x
EXEC2S = $(NAME2)_s.x
EXEC3S = $(NAME3)_s.x
# Programy wykonawcze dla biblioteki dzielonej wersja 1
EXEC1D1  = $(NAME1)_d1.x
EXEC2D1 = $(NAME2)_d1.x
EXEC3D1 = $(NAME3)_d1.x
# Programy wykonawcze dla biblioteki dzielonej wersja 2
EXEC1D2  = $(NAME1)_d2.x
EXEC2D2 = $(NAME2)_d2.x
EXEC3D2 = $(NAME3)_d2.x
# Pliki powstale po kompilacji plikow zrodlowych, tzw. object files
OBJS1  = $(NAME1).o
OBJS2 = $(NAME2).o
OBJS3 = $(NAME3).o
#------------------------------------------------------------------------------
# dla biblioteki dzielonej
OBJS5 = $(NAME5).o
OBJS6 = $(NAME6).o
LIB_NAME = _Sem_and_sm
# Katalog dla bibliotek
LIB_DIR = ./lib
# Biblioteka statyczna (static library)
LIB_STAT = lib$(LIB_NAME).a
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
LIB_OBJS = $(OBJS5) $(OBJS6)
# Pliki naglowkowe
HEADS5 = $(NAME5).h
HEADS6 = $(NAME6).h
#------------------------------------------------------------------------------
# Opcje kompilatora i linkera; -fPIC potrzebna dla bibliotek dzielonych; -D_XOPEN_SOURCE=500 dla ftruncate()
CFLAGS = -Wall -std=c99 -pedantic -O -fPIC -D_XOPEN_SOURCE=500
LFLAGS = -Wall -std=c99 -pedantic -O -D_XOPEN_SOURCE=500
# Biblioteki linkera zainstalowane w systemie 
# (tu: biblioteka semaforow)
PDLIBS = -pthread
# (tu: dolaczenie biblioteki librt dla pamieci dzielonej)
SHLIBS = -lrt
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
# Tworzenie bibliteki statycznej
$(LIB_STAT): $(LIB_OBJS)
	$(AR) $(ARFLAGS) $@ $?
	mkdir -p $(LIB_DIR)
	mv $(LIB_STAT) $(LIB_DIR)
###############################################################################
# Tworzenie biblioteki dzielonej (shared library)
$(LIB_SHAR): $(LIB_OBJS)
# Linux - opcja linkera: -shared
ifeq ($(SYSNAME), Linux)
	$(LD) -shared -o $@ $(CFLAGS) $(LIB_OBJS)
endif
# macOS - opcje linkera: -dynamiclib,  -install_name @rpath/$(LIB_DIR)/$@
ifeq ($(SYSNAME), Darwin)
	$(LD) -dynamiclib -o $@ $(CFLAGS) $(LIB_OBJS) -install_name @rpath/$(LIB_DIR)/$@
endif
	mkdir -p $(LIB_DIR)
	mv $(LIB_SHAR) $(LIB_DIR)
###############################################################################
# Plik wykonawczy z biblioteka statyczna
$(EXEC1S): $(OBJS1) $(LIB_STAT)
	rm -f $(LIB_DIR)/$(LIB_SHAR)
	$(LD) -o $@ $(LFLAGS) $(OBJS1) -L$(LIB_DIR) -l$(LIB_NAME) $(PDLIBS) $(SHLIBS)
$(EXEC2S): $(OBJS2) $(LIB_STAT)
	$(LD) -o $@ $(LFLAGS) $(OBJS2) -L$(LIB_DIR) -l$(LIB_NAME) $(PDLIBS) $(SHLIBS)
$(EXEC3S): $(OBJS3) $(LIB_STAT)
	$(LD) -o $@ $(LFLAGS) $(OBJS3) -L$(LIB_DIR) -l$(LIB_NAME) $(PDLIBS) $(SHLIBS)
###############################################################################	
#  Plik wykonawczy z biblioteka dzielona - wersja 1
#  -> informacja o lokalizacji bibliteki przekazywana jest przez opcje linkera:
#     -Wl,-R $(LIB_DIR)  (zamiast -R mozna uzyc -rpath) dla Linuksa.       
$(EXEC1D1): $(OBJS1) $(LIB_SHAR)
ifeq ($(SYSNAME), Linux)
	$(LD) -o $@ $(CFLAGS) $(OBJS1) -L$(LIB_DIR) -l$(LIB_NAME) $(PDLIBS) $(SHLIBS) -Wl,-R $(LIB_DIR)
endif
# macOS: opcje -Xlinker -rpath -Xlinker $(CURDIR)
ifeq ($(SYSNAME), Darwin)
	$(LD) -o $@ $(CFLAGS) $(OBJS1) -L$(LIB_DIR) -l$(LIB_NAME) $(PDLIBS) $(SHLIBS) -Xlinker -rpath -Xlinker $(CURDIR)
endif
$(EXEC2D1): $(OBJS2) $(LIB_SHAR)
ifeq ($(SYSNAME), Linux)
	$(LD) -o $@ $(CFLAGS) $(OBJS2) -L$(LIB_DIR) -l$(LIB_NAME) $(PDLIBS) $(SHLIBS) -Wl,-R $(LIB_DIR)
endif
# macOS: opcje -Xlinker -rpath -Xlinker $(CURDIR)
ifeq ($(SYSNAME), Darwin)
	$(LD) -o $@ $(CFLAGS) $(OBJS2) -L$(LIB_DIR) -l$(LIB_NAME) $(PDLIBS) $(SHLIBS) -Xlinker -rpath -Xlinker $(CURDIR)
endif
$(EXEC3D1): $(OBJS3) $(LIB_SHAR)
ifeq ($(SYSNAME), Linux)
	$(LD) -o $@ $(CFLAGS) $(OBJS3) -L$(LIB_DIR) -l$(LIB_NAME) $(PDLIBS) $(SHLIBS) -Wl,-R $(LIB_DIR)
endif
# macOS: opcje -Xlinker -rpath -Xlinker $(CURDIR)
ifeq ($(SYSNAME), Darwin)
	$(LD) -o $@ $(CFLAGS) $(OBJS3) -L$(LIB_DIR) -l$(LIB_NAME) $(PDLIBS) $(SHLIBS) -Xlinker -rpath -Xlinker $(CURDIR)
endif
###############################################################################	
#-------------------------------------------------------------------------------------------------
#  Plik wykonawczy z biblioteka dzielona - wersja 2
#  -> informacja o lokalizacji biblioteki nie jest przekazywana przez opcje 
#     linkera, lecz na etapie wykonania przez zmienna srodowiskowa:
#     LD_LIBRARY_PATH (patrz ponizej na przyklad uruchomienia programu).
$(EXEC1D2): $(OBJS1) $(LIB_SHAR)
	$(LD) -o $@ $(CFLAGS) $(OBJS1) -L$(LIB_DIR) -l$(LIB_NAME) $(PDLIBS) $(SHLIBS)
$(EXEC2D2): $(OBJS2) $(LIB_SHAR)
	$(LD) -o $@ $(CFLAGS) $(OBJS2) -L$(LIB_DIR) -l$(LIB_NAME) $(PDLIBS) $(SHLIBS)
$(EXEC3D2): $(OBJS3) $(LIB_SHAR)
	$(LD) -o $@ $(CFLAGS) $(OBJS3) -L$(LIB_DIR) -l$(LIB_NAME) $(PDLIBS) $(SHLIBS)

###############################################################################
# Dodatkowe reguly zaleznosci plikow objectowych od naglowkowych, ktorych
# nie uwglednia regula wzorcowa (powyzej).
###############################################################################
$(OBJS1): $(HEADS5) $(HEADS6)
$(OBJS2): $(HEADS5) $(HEADS6)
$(OBJS3): $(HEADS5) $(HEADS6)

###############################################################################
.PHONY: run1s run1d1 run1d2
############################################################################### 
# z biblioteka statyczna:
run1s: $(EXEC1S) $(EXEC2S) $(EXEC3S)
	./$(EXEC1S) $(EXEC2S) $(EXEC3S) $(DANE) $(WYNIKI)
	diff -s $(DANE) $(WYNIKI) 
############################################################################### 
#  z biblioteka dzielona  wersja 1:
run1d1: $(EXEC1D1) $(EXEC2D1) $(EXEC3D1)
	./$(EXEC1D1) $(EXEC2D1) $(EXEC3D1) $(DANE) $(WYNIKI)
	diff -s $(DANE) $(WYNIKI) 
# z biblioteka dzielona wersja 2:
run1d2: $(EXEC1D2) $(EXEC2D2) $(EXEC3D2)
# Linux
ifeq ($(SYSNAME), Linux)
	export LD_LIBRARY_PATH=${LIB_DIR}:${LD_LIBRARY_PATH}; \
	./$(EXEC1D2) $(EXEC2D2) $(EXEC3D2) $(DANE) $(WYNIKI)
	diff -s $(DANE) $(WYNIKI)
endif
# macOS - tu odpowiednia zmienna srodowiskowa to: DYLD_LIBRARY_PATH
ifeq ($(SYSNAME), Darwin)
	export DYLD_LIBRARY_PATH=${LIB_DIR}:${LD_LIBRARY_PATH}; \
	./$(EXEC1D2) $(EXEC2D2) $(EXEC3D2) $(DANE) $(WYNIKI)
	diff -s $(DANE) $(WYNIKI)
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
