obj-m := intel-ipu6.o 
EXTRA_CFLAGS += -I /home/mike/otocam-drivers/ipu6/ipu6 
EXTRA_CFLAGS += -I /home/mike/otocam-drivers/ipu6 

intel-ipu6-objs += ipu.o \
				   ipu-bus.o \
				   ipu-buttress.o \
				   ipu-cpd.o \
				   ipu-dma.o \
				   ipu-fw-com.o \
				   ipu-mmu.o \
				   ipu-trace.o \
				   ipu6/ipu6.o \

KERNEL?=/lib/modules/$(shell uname -r)/build

all:
	make -C $(KERNEL) M=$(PWD) modules 

clean:
	make -C $(KERNEL) M=$(PWD) clean
