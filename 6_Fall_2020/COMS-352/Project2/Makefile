KEEP_FILES = ! -name Assignment ! -name src ! -name Makefile ! -name . ! -name .gitignore ! -name Readme ! -name test
NETID = netid

THREADS = obj/threads/encrypt352.o obj/threads/threads.o
BUFFER = obj/util/buffer.o
UTIL = obj/util/errors.o obj/util/print_counts.o

ENCRYPT = obj/supplied/encrypt.o

PREREQS = $(THREADS) $(BUFFER) $(UTIL) $(ENCRYPT)

CFLAGS = -c -g

OUT = encrypt352

all: encrypt352

test: encrypt352
	./test/test.py

encrypt352: $(PREREQS)
	gcc -pthread -o $(OUT) $(PREREQS)

obj/threads/%.o: src/threads/%.c | obj/threads
	gcc $(CFLAGS) $< -o $@

obj/threads:
	mkdir -p $@

obj/util/%.o: src/util/%.c | obj/util
	gcc $(CFLAGS) $< -o $@

obj/util:
	mkdir -p $@

obj/supplied/%.o: src/supplied/%.c | obj/supplied
	gcc $(CFLAGS) $< -o $@

obj/supplied:
	mkdir -p $@

clean:
	find . -maxdepth 1 $(KEEP_FILES) -type f,d -exec rm -rf {} +

zip:
	mkdir $(NETID)
	cp -rt $(NETID)/ Readme Makefile src
	zip -r $(NETID) $(NETID)
	rm -rf $(NETID)
