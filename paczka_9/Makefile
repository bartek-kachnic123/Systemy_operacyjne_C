.SUFFIXES: .c .cpp .o .h .x
# Aktualny katalog
DIR = $(notdir $(CURDIR))
# Nazwa jadra systemu operacyjnego
SYSNAME = $(shell uname -s)
# Nazwy plikow
NAME1 = main
# Objects file:
OBJS1 = $(NAME1).o
# Pliki wykonawcze
EXEC1 = $(NAME1).x
#------------------------------------------------------------------------------
# Opcje kompilatora i linkera
CFLAGS = -Wall -std=c99 -pedantic -O
# Biblioteka dla watkow
LPTLIBS = -pthread -lpthread 
# Kompilator i linker 
CO = gcc
LD = $(CO)
# Opcje archiwizera: tworzenie/uaktualnianie biblioteki wraz z indeksem plikow
#                    i wypisywanie komunikatow 
ARFLAGS = rsv
###############################################################################
# Reguly zaleznosci
%.o: %.c %.h
	$(CO) $(CFLAGS) -c $<
# Jak wyzej, ale bez zaleznosci od plikow naglowkowych
%.o: %.c
	$(CO) $(CFLAGS) -c $<
###############################################################################
$(EXEC1): $(OBJS1)
	$(LD) -o $@ $(CFLAGS) $(LPTLIBS) $^

.PHONY: run1
# Uruchomienie programu
run1: $(EXEC1)
	./$(EXEC1)

###############################################################################
# Sprzataczka (tez "phony target")
###############################################################################
.PHONY: clean tar
clean:                                                     
	rm -f *.o  *~ *.a *.so *.x core core* a.out; rm -rf ${LIB_DIR}
# Archiwizacja i kompresja
tar: clean
	(cd ../; tar -cvzf $(DIR).tar.gz  $(DIR) )