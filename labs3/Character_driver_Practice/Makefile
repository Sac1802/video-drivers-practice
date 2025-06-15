obj-m += practice.o

KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

all:
	make -C $(KADIR) M=$(PWD) modules

clean:
	make -C $(KADIR) M=$(PWD) clean