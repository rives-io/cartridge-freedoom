FREEDOOM_VER=v0.4.0
RIV_VER=v0.3-rc16
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

image: rivos.ext2 freedoom.sqfs
	cartesi-machine \
		--user=root \
		--append-entrypoint-file=entrypoint.sh \
		--flash-drive=start:0x80000000000000,label:root,filename:rivos.ext2 \
		--flash-drive=start:0x90000000000000,label:cartridge,filename:freedoom.sqfs,mount:false \
		--flash-drive=start:0xa0000000000000,length:64Ki,label:tape \
		--flash-drive=start:0xb0000000000000,length:64Ki,label:outcard \
		--max-mcycle=0 \
		--store=image

rivos.ext2:
	wget -O rivos.ext2 https://github.com/rives-io/riv/releases/download/$(RIV_VER)/rivos.ext2

freedoom.sqfs:
	wget -O freedoom.sqfs https://github.com/rives-io/cartridge-freedoom/releases/download/$(FREEDOOM_VER)/freedoom.sqfs

clean:
	rm -rf *.tmp *.json image

distclean: clean
	rm -f rivos.ext2 freedoom.sqfs out
