FREEDOOM_VER=v0.3.0
KERNEL_VER=v0.19.1-riv1
RIV_VER=v0.3-rc12
LINUX_VER=linux-6.5.9-ctsi-1-$(KERNEL_VER)
TAPE=gameplay-example.rivtape

all: image

compute: image
	@cp $(TAPE) $(TAPE).tmp && truncate -s 64K $(TAPE).tmp
	@rm -f $(TAPE).json && truncate -s 64K $(TAPE).json
	cartesi-machine \
		--load=image \
		--replace-flash-drive=start:0xa0000000000000,length:64Ki,filename:$(TAPE).tmp \
		--replace-flash-drive=start:0xb0000000000000,length:64Ki,filename:$(TAPE).json,shared
	@rm -f $(TAPE).tmp
	@echo ==== OUTCARD ====
	@cat $(TAPE).json
	@echo

image: rivos.ext2 kernel.bin freedoom.sqfs
	cartesi-machine \
		--ram-image=kernel.bin \
		--append-init=USER=root \
		--flash-drive=start:0x80000000000000,label:root,filename:rivos.ext2 \
		--flash-drive=start:0x90000000000000,label:cartridge,filename:freedoom.sqfs,mount:false \
		--flash-drive=start:0xa0000000000000,length:64Ki,label:tape \
		--flash-drive=start:0xb0000000000000,length:64Ki,label:outcard \
		--max-mcycle=0 \
		--store=image \
		--append-entrypoint-file=entrypoint.sh

rivos.ext2:
	wget -O rivos.ext2 https://github.com/rives-io/riv/releases/download/$(RIV_VER)/rivos.ext2

kernel.bin:
	wget -O kernel.bin https://github.com/rives-io/kernel/releases/download/$(KERNEL_VER)/$(LINUX_VER).bin

freedoom.sqfs:
	wget -O freedoom.sqfs https://github.com/rives-io/cartridge-freedoom/releases/download/$(FREEDOOM_VER)/freedoom.sqfs

clean:
	rm -rf *.tmp *.json image

distclean: clean
	rm -f rivos.ext2 kernel.bin freedoom.sqfs out
