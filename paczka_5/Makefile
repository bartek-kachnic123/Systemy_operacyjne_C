.SUFFIXES: .c .cpp .o .x .h
#==============================================================================
DIR = $(notdir $(CURDIR))
SYSNAME = $(shell uname -s)
#==============================================================================
NAME1 = prog_
NAME2 = producent
NAME3 = konsument
NAME4 = dane
NAME5 = wyniki
NAME6 = fifo
#==============================================================================
# Pliki tekstowe:
DANE = $(NAME4).txt
WYNIKI = $(NAME5).txt
#==============================================================================
# Plik potoku
FIFO = $(NAME6)_1
FIFO_2 = $(NAME6)_2
#==============================================================================
#==============================================================================
# Programy wykonawcze:
EXEC1  = $(NAME1)1.x  
EXEC2 = $(NAME2).x
EXEC3 = $(NAME3).x

#==============================================================================
# Pliki powstale po kompilacji plikow zrodlowych, tzw. object files
OBJS1  = $(NAME1)1.o
OBJS2 = $(NAME2).o
OBJS3 = $(NAME3).o

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
#==============================================================================
.PHONY: run1 run2 
#==============================================================================
run1: $(EXEC1) $(EXEC2) $(EXEC3)
	./$(EXEC1) $(EXEC2) $(EXEC3) $(FIFO) $(DANE) $(WYNIKI)
	diff -s $(DANE) $(WYNIKI)
run2: $(EXEC2) $(EXEC3)
	mkfifo -m 0777 $(FIFO_2)
	xterm -hold -title PRODUCENT -e ./$(EXEC2) $(FIFO_2) $(DANE) &
	xterm -hold -title KONSUMENT -e ./$(EXEC3) $(FIFO_2) $(WYNIKI) 
	rm $(FIFO_2)
	diff -s $(DANE) $(WYNIKI)
	


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
