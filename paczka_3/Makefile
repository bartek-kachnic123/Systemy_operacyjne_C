.SUFFIXES: .c .cpp .o .x .h
#==============================================================================
DIR = $(notdir $(CURDIR))
SYSNAME = $(shell uname -s)
#==============================================================================
NAME1 = prog_
# Nazwy sygnalow
SIGINT = 2
#==============================================================================
# Opcje obslugi synaglow
#==============================================================================
# Domyslna obsluga sygnalu
OPCJA1 = 1
# Ignorowanie sygnalu
OPCJA2 = 2
# Wlasna obsluga sygnalu
OPCJA3 = 3
#==============================================================================
# Programy wykonawcze:
EXEC1  = $(NAME1)1.x  
EXEC2  = $(NAME1)2.x 
EXEC3  = $(NAME1)3.x 
EXEC4  = $(NAME1)3_pomoc.x 
#==============================================================================
# Pliki powstale po kompilacji plikow zrodlowych, tzw. object files
OBJS1  = $(NAME1)1.o
OBJS2  = $(NAME1)2.o
OBJS3  = $(NAME1)3.o
OBJS4  = $(NAME1)3_pomoc.o
#==============================================================================
# Makra  przewidziane na przyszlosc.
LIBS1  = 
LIBS2  = 
LIBS3  = 
LIBS4  = 
#==============================================================================
# Opcje kompilatora i linkera;
LFLAGS = -Wall -std=c99 -pedantic -O
#==============================================================================
# Kompilator i linker (czesto ta sama nazwa)
CO = gcc
LD = $(CO)
#==============================================================================
%.o: %.c %.h
	$(CO) $(LFLAGS) -c $<
# Jak wyzej, ale bez zaleznosci od plikow naglowkowych
%.o: %.c
	$(CO) $(LFLAGS) -c $<
#==============================================================================
$(EXEC1): $(OBJS1) $(LIBS1) 
	$(LD) -o $@ $(LFLAGS) $^
$(EXEC2): $(OBJS2) $(LIBS2) 
	$(LD) -o $@ $(LFLAGS) $^
$(EXEC3): $(OBJS3) $(LIBS3) 
	$(LD) -o $@ $(LFLAGS) $^
$(EXEC4): $(OBJS4) $(LIBS4) 
	$(LD) -o $@ $(LFLAGS) $^		
#==============================================================================
.PHONY: run1 run2 run3 
#==============================================================================
run1: $(EXEC1) 
	./$(EXEC1) $(SIGINT) $(OPCJA1)
run2: $(EXEC2) $(EXEC1)
	./$(EXEC2) $(EXEC1) 3 $(OPCJA3)
run3: $(EXEC3) $(EXEC1) $(EXEC4)
	./$(EXEC3) $(EXEC4) $(EXEC1) 5 $(OPCJA1)
#==============================================================================

###############################################################################
# Sprzataczka (tez "phony target")
###############################################################################
.PHONY: clean tar
clean:                                                     
	rm -f *.o  *~ *.a *.so *.x core core* a.out; rm -rf ${LIB_DIR}
# Archiwizacja i kompresja
tar: clean
	(cd ../; tar -cvzf $(DIR).tar.gz  $(DIR) )
