all: main.hex

main.hex: main.elf
	avr-objcopy -O ihex main.elf main.hex

main.elf: main.c
	avr-gcc -mmcu=atmega328p main.c -o main.elf

upload: main.hex
	avrdude -p m328p -c arduino -P /dev/ttyACM0 -b 19200 -U flash:w:main.hex
