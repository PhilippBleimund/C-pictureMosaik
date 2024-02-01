ifdef OS
   RM = del /Q
   flags :=
   ending := .exe
   preRun := 
else
   ifeq ($(shell uname), Linux)
      RM = rm -f -r
      flags := -lncurses
      preRun := ./
   endif
endif

build: src/userInterface.c
	gcc src/userInterface.c -o src/userInterface.ck $(flags)
	cd src && $(preRun)userInterface$(ending)	
