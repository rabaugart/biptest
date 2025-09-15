# name of your application
APPLICATION = ws2812_alt

# If no BOARD is found in the environment, use this default:
BOARD ?= atmega328p

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(HOME)/dev/RIOT

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
DEVELHELP ?= 1

USEMODULE += periph_gpio
USEMODULE += periph_gpio_irq
USEMODULE += lpd8808
# pcint2 funktioniert nicht, wie gedacht
# USEMODULE += atmega_pcint2

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

FEATURES_REQUIRED += cpp

# Use a peripheral timer for the delay, if available
FEATURES_OPTIONAL += periph_timer

include $(RIOTBASE)/Makefile.include
fl: bin/atmega328p/ws2812_alt.hex
	avrdude -c arduino -p m328p -P /dev/ttyACM0 -U flash:w:bin/atmega328p/ws2812_alt.hex
