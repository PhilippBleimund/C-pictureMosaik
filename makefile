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
	mkdir build
	gcc src/userInterface.c -o build/userInterface.out $(flags)
	cd build && $(preRun)userInterface$(ending)	
