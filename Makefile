# C sources code for DOOM
OBJS=\
	src/am_map.o \
	src/d_event.o \
	src/d_items.o \
	src/d_iwad.o \
	src/d_loop.o \
	src/d_main.o \
	src/d_mode.o \
	src/d_net.o \
	src/doomdef.o \
	src/doomstat.o \
	src/dstrings.o \
	src/dummy.o \
	src/f_finale.o \
	src/f_wipe.o \
	src/g_game.o \
	src/hu_lib.o \
	src/hu_stuff.o \
	src/i_cdmus.o \
	src/i_endoom.o \
	src/i_joystick.o \
	src/i_main.o \
	src/i_scale.o \
	src/i_system.o \
	src/i_timer.o \
	src/info.o \
	src/m_argv.o \
	src/m_bbox.o \
	src/m_cheat.o \
	src/m_config.o \
	src/m_controls.o \
	src/m_fixed.o \
	src/m_menu.o \
	src/m_misc.o \
	src/m_random.o \
	src/memio.o \
	src/p_ceilng.o \
	src/p_doors.o \
	src/p_enemy.o \
	src/p_floor.o \
	src/p_inter.o \
	src/p_lights.o \
	src/p_map.o \
	src/p_maputl.o \
	src/p_mobj.o \
	src/p_plats.o \
	src/p_pspr.o \
	src/p_saveg.o \
	src/p_setup.o \
	src/p_sight.o \
	src/p_spec.o \
	src/p_switch.o \
	src/p_telept.o \
	src/p_tick.o \
	src/p_user.o \
	src/r_bsp.o \
	src/r_data.o \
	src/r_draw.o \
	src/r_main.o \
	src/r_plane.o \
	src/r_segs.o \
	src/r_sky.o \
	src/r_things.o \
	src/s_sound.o \
	src/sha1.o \
	src/sounds.o \
	src/st_lib.o \
	src/st_stuff.o \
	src/statdump.o \
	src/tables.o \
	src/v_video.o \
	src/w_checksum.o \
	src/w_file.o \
	src/w_file_stdc_unbuffered.o \
	src/w_main.o \
	src/w_wad.o \
	src/wi_stuff.o \
	src/z_zone.o \
	src/i_input_riv.o \
	src/i_rivsound.o \
	src/i_rivmusic.o \
	src/i_sound_riv.o \
	src/i_video_riv.o

# Asset files to be added to the cartridge
ASSETS=\
	0-entry.sh \
	freedoom1.wad \
	info.json

# Executable file to compile
EXE=doom.elf

# Cartridge file to generate
CARTRIDGE=freedoom.sqfs

# Cartridge compression algorithm
COMPRESSION=lzo

# RIVEMU exec shortcut for its SDK
RIVEMU_RUN=rivemu
RIVEMU_EXEC=rivemu -quiet -no-window -sdk -workspace -exec
ifneq (,$(wildcard /usr/sbin/riv-run))
	RIVEMU_RUN=riv-run
	RIVEMU_EXEC=
endif

# Compilation flags
CFLAGS=$(shell $(RIVEMU_EXEC) riv-opt-flags -Ospeed --cflags)
LDFLAGS=$(shell $(RIVEMU_EXEC) riv-opt-flags -Ospeed --ldflags)

build: $(CARTRIDGE)

# Test cartridge (for development)
test: $(EXE)
	$(RIVEMU_RUN) -workspace -exec ./0-entry.sh

# Run cartridge
run: $(CARTRIDGE)
	$(RIVEMU_RUN) $<

# Generate FREEDOOM cartridge
$(CARTRIDGE): $(EXE) $(ASSETS)
	$(RIVEMU_EXEC) riv-mksqfs $^ $@ -comp $(COMPRESSION)

# Compile DOOM executable
$(EXE): $(OBJS)
	$(RIVEMU_EXEC) gcc $(OBJS) -o $@ $(LDFLAGS)
	$(RIVEMU_EXEC) riv-strip $@

# Compile C source code
src/%.o: src/%.c
	$(RIVEMU_EXEC) gcc -c $< -o $@ $(CFLAGS)

# Clean
clean:
	rm -f *.sqfs *.elf src/*.o
