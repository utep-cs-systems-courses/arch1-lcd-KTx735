# makfile configuration
CPU             	= msp430g2553
CFLAGS          	= -mmcu=${CPU} -Os -I../h
LDFLAGS		= -L../lib -L/opt/ti/msp430_gcc/include/ 

#switch the compiler (for the internal make rules)
CC              = msp430-elf-gcc
AS              = msp430-elf-gcc -mmcu=${CPU} -c

all:wakedemo_c.elf wakedemo_s.elf

#additional rules for files
wakedemo_c.elf: ${COMMON_OBJECTS} wakedemo.o wdt_handler.o buzzer.o sound_c.o
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $^ -lTimer -lLcd

wakedemo_s.elf: ${COMMON_OBJECTS} wakedemo.o wdt_handler.o buzzer.o sound_s.o
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $^ -lTimer -lLcd 

load_c: wakedemo_c.elf
	msp430loader.sh $^

load_s: wakedemo_s.elf
	msp430loader.sh $^

clean:
	rm -f *.o *.elf

buzzer.o: buzzer.h
sound_c.o: sound.h
sound_s.o: sound.h
