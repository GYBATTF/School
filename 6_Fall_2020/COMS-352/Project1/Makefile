all: shell352

shell352: ./obj/input.o ./obj/shell352.o ./obj/processes.o
	gcc -o $@ $^

obj/shell352.o: ./src/shell352.c | obj
	gcc -g -c $< -o $@

obj/input.o: ./src/input.c | obj
	gcc -g -c $< -o $@

obj/processes.o: ./src/processes.c | obj
	gcc -g -c $< -o $@

obj:
	mkdir $@

clean:
	find . -maxdepth 1 ! -name Assignment ! -name src ! -name Makefile ! -name . ! -name .gitignore ! -name Readme -type f,d -exec rm -rf {} +

zip: clean
	mkdir ajharms
	cp -rt ajharms/ Readme Makefile src
	zip -r ajharms ajharms
	rm -rf ajharms
