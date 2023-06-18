main:
	sdcc -o lib/7segx4/ -c lib/7segx4/7segx4.c
	sdar -o lib/7segx4/7segx4.lib -rc lib/7segx4/7segx4.rel

	sdcc -o lib/keypad4x4/ -c lib/keypad4x4/keypad4x4.c
	sdar -o lib/keypad4x4/keypad4x4.lib -rc lib/keypad4x4/keypad4x4.rel

	sdcc main.c lib/7segx4/7segx4.lib lib/keypad4x4/keypad4x4.lib
	packihx main.ihx > main.hex

.PHONY: clean
clean:
	-del /q /s *.ihx, *.lk, *.lst, *.map, *.mem, *.rel, *.rst, *.sym, *.asm