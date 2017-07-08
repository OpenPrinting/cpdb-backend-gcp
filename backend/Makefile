CC=gcc
CFLAGS=-std=c11 `pkg-config --cflags glib-2.0 gio-2.0 gobject-2.0 rest-0.7 json-glib-1.0 goa-1.0`
LDFLAGS=`pkg-config --libs glib-2.0 gio-2.0 gobject-2.0 rest-0.7 json-glib-1.0 goa-1.0`

EXEC=main

SRC=test.c\
    gcp.c\
		utils.c

OBJ=test.o\
    gcp.o\
		utils.o

$(EXEC): $(OBJ)
	$(CC) -g -o $@ $^ $(CFLAGS) $(LDFLAGS)

%.o: %.c
	$(CC) -g -c -o $@ $< $(CFLAGS)

.PHONY: clean run

clean:
	rm -f $(OBJ) $(EXEC)

gen: genback

genback:
	gdbus-codegen --generate-c-code backend_interface  --interface-prefix org.openprinting org.openprinting.Backend.xml
