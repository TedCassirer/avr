CC=avr-gcc

OBJCOPY=avr-objcopy

CFLAGS=-g -mmcu=atmega16 -std=c11

#Info til AVRDUDE
PROGRAMMER=avrdude
BOARD=stk500
MCU=m16
PORT=/dev/ttyUSB0

PFLAGS=-p $(MCU) -c $(BOARD) -P $(PORT)

rom.hex : main.out
	$(OBJCOPY) -j .text -O ihex main.out rom.hex

main.out : main.o
	$(CC) $(CFLAGS) -o main.out -Wl,-Map,main.map main.o

main.o : main.c
	$(CC) $(CFLAGS) -Os -c main.c

clean:
	rm -f *.o *.out *.map *.hex

program: rom.hex
	sudo $(PROGRAMMER) $(PFLAGS) -e
	sudo $(PROGRAMMER) $(PFLAGS) -U flash:w:rom.hex
