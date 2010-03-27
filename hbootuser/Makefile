CC = arm-android-linux-uclibcgnueabi-gcc
STRIP = arm-android-linux-uclibcgnueabi-strip
CFLAGS = -static -Wall -g

hbootuser: hbootuser.o
	$(CC) $(CFLAGS) -o hbootuser hbootuser.o
	$(STRIP) hbootuser

clean:
	rm *.o
	rm hbootuser
