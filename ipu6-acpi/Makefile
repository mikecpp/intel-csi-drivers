obj-m := ipu6_acpi.o 
EXTRA_CFLAGS += -I /home/mike/otocam-drivers/ipu6/ipu6 
EXTRA_CFLAGS += -I /home/mike/otocam-drivers/ipu6 

ipu6_acpi-objs += ipu6-acpi.o \
				  ipu6-acpi-pdata.o \
				  ipu6-acpi-common.o

KERNEL?=/lib/modules/$(shell uname -r)/build

all:
	make -C $(KERNEL) M=$(PWD) modules 

clean:
	make -C $(KERNEL) M=$(PWD) clean
