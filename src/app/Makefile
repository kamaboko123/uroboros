# Makefile

IMG = bin/app_disk.img
APP = bin/hello

all: $(IMG)

$(IMG): $(APP)
	mkdir -p bin
	mformat -i $(IMG) -f 1440 -C ::
	mcopy -i $(IMG) -s $(APP) ::

$(APP): hello/hello.c
	mkdir -p bin
	cd hello && make
	cp hello/bin/dist/hello bin/

clean:
	rm -rf bin
	rm -f $(IMG)
	cd hello && make clean
