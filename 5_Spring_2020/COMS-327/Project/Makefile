all: bin/check bin/advance bin/winnable bin/game

## builds
bin/game: ./obj/game.oo ./obj/interface.o ./obj/game.o ./obj/scanner.o ./obj/validator.o ./obj/moves.o ./obj/card.o ./obj/output.o | bin
	g++ -o $@ $^ /share/cs327/lib/libtermbox.a

bin/check: ./obj/game.o ./obj/scanner.o ./obj/validator.o ./obj/moves.o ./obj/card.o ./obj/output.o ./obj/check.o | bin
	gcc -o $@ $^

bin/advance: ./obj/game.o ./obj/scanner.o ./obj/validator.o ./obj/moves.o ./obj/card.o ./obj/output.o ./obj/advance.o | bin
	gcc -o $@ $^

bin/winnable: ./obj/solve.o ./obj/game.o ./obj/scanner.o ./obj/validator.o ./obj/moves.o ./obj/card.o ./obj/output.o ./obj/winnable.o | bin
	g++ -o $@ $^

 
## main
obj/game.oo: ./MAIN/game.cc | obj
	g++ -c $< -o $@

obj/check.o: ./MAIN/check.c | obj
	gcc -c $< -o $@

obj/advance.o: ./MAIN/advance.c | obj
	gcc -c $< -o $@

obj/winnable.o: ./MAIN/winnable.cc | obj
	g++ -c $< -o $@
 
## game
obj/game.o: ./MODULES/game/game.c | obj
	gcc -c $< -o $@

obj/card.o: ./MODULES/game/card.c | obj
	gcc -c $< -o $@

## io
obj/output.o: ./MODULES/io/output.c | obj
	gcc -c $< -o $@

obj/scanner.o: ./MODULES/io/scanner.c | obj
	gcc -c $< -o $@

obj/validator.o: ./MODULES/io/validator.c | obj
	gcc -c $< -o $@

obj/interface.o: ./MODULES/io/interface.cc | obj
	g++ -c $< -o $@ -I/share/cs327/include

## moves
obj/moves.o: ./MODULES/moves/moves.c | obj
	gcc -c $< -o $@

obj/solve.o: ./MODULES/moves/solve.cc | obj
	g++ -c $< -o $@

# create object folder
obj:
	mkdir $@

# create binary's folder
bin:
	mkdir $@

# clean
clean:
	rm -rf obj bin game check advance winnable
