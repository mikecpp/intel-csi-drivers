obj-m := intel-ipu6-isys.o 
EXTRA_CFLAGS += -I /home/mike/otocam-drivers/ipu6/ipu6 
EXTRA_CFLAGS += -I /home/mike/otocam-drivers/ipu6 

intel-ipu6-isys-objs += ipu-isys.o \
					    ipu-isys-csi2.o \
					    ipu-isys-csi2-be-soc.o \
					    ipu-fw-isys.o \
					    ipu-isys-video.o \
					    ipu-isys-queue.o \
					    ipu-isys-subdev.o \
					    ipu-isys-csi2-be.o \
					    ipu6/ipu6-isys.o \
					    ipu6/ipu6-isys-phy.o \
					    ipu6/ipu6-isys-dwc-phy.o \
					    ipu6/ipu6-isys-csi2.o \
					    ipu6/ipu6-isys-gpc.o \

KERNEL?=/lib/modules/$(shell uname -r)/build

all:
	make -C $(KERNEL) M=$(PWD) modules 

clean:
	make -C $(KERNEL) M=$(PWD) clean
