CC = arm-android-linux-uclibcgnueabi-gcc
STRIP = arm-android-linux-uclibcgnueabi-strip
CFLAGS = -static -Wall -g

hbootuser: hbootuser.o
	$(CC) $(CFLAGS) -o 2ndboot hbootuser.o
	$(STRIP) 2ndboot

clean:
	rm *.o
	rm 2ndboot
