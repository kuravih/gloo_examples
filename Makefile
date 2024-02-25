all: 00_simple/00_simple

00_simple/00_simple:
	make -C "./00_simple"

clean:
	make clean -C "./00_simple"