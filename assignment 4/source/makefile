MPICC	= mpicc

TARGETS = gameoflife

all : $(TARGETS)
	@echo Make Complete

gameoflife : gameoflife.c $(SPECDEP)
	$(MPICC) gameoflife.c -o gameoflife

clean :
	rm gameoflife
	@echo Makeclean Complete
