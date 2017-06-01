CFLAGS ?= -O3 -g -Wall

#%.o : %.c %.h
#	$(CC) $(CFLAGS) -fPIC -c $<

asp-icc-extractor: asp-icc-extractor.c
	$(CC) $(CFLAGS) -o $@ $< -L.

clean:
	rm -f asp-icc-extractor *.o
	rm -f *.orig

astyle:
	astyle --options=astylerc *.c
