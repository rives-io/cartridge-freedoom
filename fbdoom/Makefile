################################################################
#
# $Id:$
#
# $Log:$
#

CROSS_COMPILE ?= riscv64-cartesi-linux-gnu-
NOSDL=1

ifeq ($(V),1)
	VB=''
else
	VB=@
endif

#LIBS+=-lXext -lX11 -lnsl -lm -lSDL
#CFLAGS+=-Wunused-const-variable=0 
#CFLAGS+=-fsanitize=address
OBJS+=$(OBJDIR)/i_video_riv.o
OBJS+=$(OBJDIR)/i_sound_riv.o
OBJS+=$(OBJDIR)/i_rivsound.o
OBJS+=$(OBJDIR)/i_input_riv.o

CC=$(CROSS_COMPILE)gcc  # gcc or g++
CFLAGS+=-Ofast -flto=auto -fno-stack-protector -ffunction-sections -fdata-sections -DNDEBUG
LDFLAGS+=-Wl,-O1,-gc-sections,--as-needed,--no-eh-frame-hdr,--build-id=none,--hash-style=gnu,--relax,--sort-common -z relro -z now -s
CFLAGS+=-Wall -DNORMALUNIX -DLINUX #-DSNDSERV -DUSEASM
LIBS+=-lm -lc -L../build -lriv

ifneq ($(NOSDL),1)
	LIBS+= -lSDL
endif

# subdirectory for objects
OBJDIR=../build/fbdoom_objs/
OUTPUT=../build/fbdoom

SRC_DOOM = i_main.o dummy.o am_map.o doomdef.o doomstat.o dstrings.o d_event.o d_items.o d_iwad.o d_loop.o d_main.o d_mode.o d_net.o f_finale.o f_wipe.o g_game.o hu_lib.o hu_stuff.o info.o i_cdmus.o i_endoom.o i_joystick.o i_scale.o i_system.o i_timer.o memio.o m_argv.o m_bbox.o m_cheat.o m_config.o m_controls.o m_fixed.o m_menu.o m_misc.o m_random.o p_ceilng.o p_doors.o p_enemy.o p_floor.o p_inter.o p_lights.o p_map.o p_maputl.o p_mobj.o p_plats.o p_pspr.o p_saveg.o p_setup.o p_sight.o p_spec.o p_switch.o p_telept.o p_tick.o p_user.o r_bsp.o r_data.o r_draw.o r_main.o r_plane.o r_segs.o r_sky.o r_things.o sha1.o sounds.o statdump.o st_lib.o st_stuff.o s_sound.o tables.o v_video.o wi_stuff.o w_checksum.o w_file.o w_file_stdc_unbuffered.o w_main.o w_wad.o z_zone.o
OBJS += $(addprefix $(OBJDIR)/, $(SRC_DOOM))

all: $(OUTPUT)

clean:
	rm -rf $(OBJDIR)
	rm -f $(OUTPUT)

$(OUTPUT): $(OBJS) Makefile
	@echo [Linking $@]
	$(VB)$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) \
	-o $(OUTPUT) $(LIBS)
	@echo [Size]
	-$(CROSS_COMPILE)size $(OUTPUT)

$(OBJS): | $(OBJDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/%.o:	%.c
	@echo [Compiling $<]
	$(VB)$(CC) $(CFLAGS) -c $< -o $@

print:
	@echo OBJS: $(OBJS)

