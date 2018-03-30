TARGET:=oled_user
SRCS:=oled_userspace_skel.c
OBJS:=$(SRCS:.c=.o)
CFLAGS:= -g
CROSS_COMPILE?=/ECEnet/Apps1/esl/gcc-7.2.1-arm/bin/arm-linux-gnueabihf
UPLOAD_PATH ?= /home/root/
ZEDBOARD_IP?=qemu

.PHONY: clean upload

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CROSS_COMPILE)-gcc -o $@ $^

%.o: %.c %.h
	$(CROSS_COMPILE)-gcc $(CFLAGS) -c $<

%.o: %.c
	$(CROSS_COMPILE)-gcc $(CFLAGS) -c $<

clean:
	rm -rf $(OBJS) $(TARGET)

upload: $(TARGET)
	scp $(TARGET) root@$(ZEDBOARD_IP):$(UPLOAD_PATH)
