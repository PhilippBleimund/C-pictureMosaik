ifdef OS
   RM = del /Q
   flags :=
   ending := .exe
   preRun := 
else
   ifeq ($(shell uname), Linux)
      RM = rm -f -r
      flags := -lncurses -lm
			ending := .out
      preRun := ./
   endif
endif

libs := src/lib/progressbar.c src/lib/statusbar.c

objects := src/userInterface.c src/image.c src/database.c

run: all 
	cd build && $(preRun)userInterface$(ending)

all: 
	mkdir -p build
	gcc $(objects) $(libs) -o build/userInterface.out -Wall $(flags)
	

debug:
	mkdir -p build
	gcc --debug $(objects) $(libs) -o build/outDebug -Wall $(flags)
