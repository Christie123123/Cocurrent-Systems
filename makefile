calculate: .FORCE
	gcc -o calculate calculate.c log.c -D_multithread -lpthread
.FORCE:
