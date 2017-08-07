FLAGS=$(shell pkg-config --libs --cflags gio-unix-2.0 glib-2.0 gio-2.0 gobject-2.0 rest-0.7 json-glib-1.0 goa-1.0)
.PHONY:all gen lib

all: client server

gen:genback

genback:
	gdbus-codegen --generate-c-code backend_interface  --interface-prefix org.openprinting ../interface/org.openprinting.Backend.xml

%.o: %.c
	gcc -g -o $@ $^ -c $(FLAGS)

server: gcp_server.o backend_interface.o common_helper.o gcp.o
	gcc -g -o $@ $^ $(FLAGS)

client: client.o backend_interface.o common_helper.o
	gcc -g -o $@ $^ $(FLAGS)

install:
	./install.sh

clean_gen:
	rm -f backend_interface.*

clean:
	rm -f *.o client server
