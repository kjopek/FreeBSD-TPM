# $FreeBSD: release/10.0.0/sys/boot/i386/pmbr/Makefile 208789 2010-06-03 17:42:32Z ed $

PROG=	pmbr
STRIP=
BINMODE=${NOBINMODE}
NO_MAN=
SRCS=	pmbr.s

ORG=	0x600

AFLAGS+=--defsym FLAGS=${BOOT_MBR_FLAGS} -g
LDFLAGS=-e start -Ttext ${ORG} -Wl,-N,-S,--oformat,binary -nostdlib

test_disk:
	sudo dd if=/dev/zero of=/dev/da0 bs=1M count=1
	sudo gpart create -s GPT /dev/da0
	sudo gpart add -t freebsd-boot -s 64k /dev/da0
	sudo gpart add -t freebsd-ufs /dev/da0
	sudo gpart bootcode -b ./pmbr -p /boot/gptboot -i 1 /dev/da0
	sudo newfs /dev/da0p2
	sudo mkdir -p /tmp/mnt
	sudo mount /dev/da0p2 /tmp/mnt
	sudo mkdir -p /tmp/mnt/boot/kernel
	sudo cp -fr /boot/loader /tmp/mnt/boot
	sudo cp -fr /boot/kernel/kernel /tmp/mnt/boot/kernel/
	sync
	sudo umount -f /tmp/mnt
	sudo rmdir /tmp/mnt

test_image:
	rm -fr image
	mkdir image
	dd if=/dev/zero of=image/disk.img bs=1M count=30
	sudo mdconfig -a -t vnode -f image/disk.img
	sudo gpart create -s GPT /dev/md0
	sudo gpart add -t freebsd-boot -s 128k /dev/md0
	sudo gpart add -t freebsd-ufs /dev/md0
	sudo gpart bootcode -b ./pmbr -p ./gptboot -i 1 /dev/md0
	sudo newfs /dev/md0p2
	sudo mkdir -p /tmp/mnt
	sudo mount /dev/md0p2 /tmp/mnt
	sudo mkdir -p /tmp/mnt/boot/kernel
	sudo cp -fr /boot/loader /tmp/mnt/boot
	sudo cp -fr /boot/kernel/kernel /tmp/mnt/boot/kernel/
	sync
	sudo umount -f /tmp/mnt
	sudo mdconfig -d -u 0

.include <bsd.prog.mk>
