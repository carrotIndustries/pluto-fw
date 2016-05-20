SMAPS_C = hal/segmap.c
GEN_SMAP = hal/gen_map.py

SRC += hal/lcd.c \
       hal/wdt.c \
       hal/io.c \
       hal/rtc.c \
       hal/button.c \
       hal/beepled.c \
       hal/aux_timer.c \
       hal/i2c.c \
       hal/mag3110.c \
       hal/compass.c \
       hal/aes/aes256.c \
       hal/crypto.c \
       $(SMAPS_C)

$(SMAPS_C): %.c: %.map $(GEN_SMAP)
	$(GEN_SMAP) $< > $@
