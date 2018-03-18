ASM2OBJ = vc +jaguar $? -c -o $@
C2OBJ = vc +jaguar -O2 -c -c99 -o $@ $?
AR = ar
AROPTS = qS
OBJPATH = obj/
LIBPATH = lib/

#jaglib-lib-start
JAGLIB1 = $(OBJPATH)skunk.o $(OBJPATH)skunkTimeout.o $(OBJPATH)debug.o $(OBJPATH)wait_blitter_ready.o $(OBJPATH)blit_char.o \
		$(OBJPATH)memset8.o $(OBJPATH)memset32.o $(OBJPATH)memcpy16p.o  $(OBJPATH)memcpy32p.o \
		$(OBJPATH)set_olp.o $(OBJPATH)attach_olp.o $(OBJPATH)append_olp.o $(OBJPATH)restore_ol.o $(OBJPATH)set_indexed_color.o \
		$(OBJPATH)console_save_bmp.o $(OBJPATH)console_hide.o $(OBJPATH)console_show.o $(OBJPATH)console_hide_startup.o

JAGLIB2 = $(OBJPATH)libmain.o $(OBJPATH)global_interrupt_handler.o $(OBJPATH)custom_interrupt_handler.o $(OBJPATH)welcome_message.o $(OBJPATH)init_message.o \
		$(OBJPATH)io_open.o $(OBJPATH)jagopen.o $(OBJPATH)io_close.o $(OBJPATH)jagclose.o $(OBJPATH)io_seek.o $(OBJPATH)jagseek.o \
		$(OBJPATH)io_read.o $(OBJPATH)jagread.o $(OBJPATH)io_write.o $(OBJPATH)jagwrite.o $(OBJPATH)putc.o \
		$(OBJPATH)read_stick0.o $(OBJPATH)read_stick1.o $(OBJPATH)audio_mute.o \
		$(OBJPATH)dsp.o $(OBJPATH)gpu.o
#jaglib-lib-end

all: buildlib

# deploy to target. Call manually
postcopy:
	cp -f $(LIBPATH)libjag.a $(VBCC)/targets/m68k-jaguar/lib/
	cp -f jagcore.h $(VBCC)/targets/m68k-jaguar/include/
	cp -f jaglib.h $(VBCC)/targets/m68k-jaguar/include/

buildlib: $(JAGLIB1) $(JAGLIB2)
	rm $(LIBPATH)libjag.a -f
	$(AR) $(AROPTS) $(LIBPATH)libjag.a $(JAGLIB)

clean:
	-rm doc/ -r -f
	rm $(OBJPATH)*.o -f
	rm $(LIBPATH)libjag.a -f

#jaglib-files-start
#
# 68k jaguar specific asm files
#
$(OBJPATH)debug.o: debug.s
	$(ASM2OBJ)
$(OBJPATH)skunk.o: skunk.s
	$(ASM2OBJ)
$(OBJPATH)memset8.o: memset8.s
	$(ASM2OBJ)
$(OBJPATH)memset32.o: memset32.s
	$(ASM2OBJ)
$(OBJPATH)memcpy16p.o: memcpy16p.s
	$(ASM2OBJ)
$(OBJPATH)memcpy32p.o: memcpy32p.s
	$(ASM2OBJ)
$(OBJPATH)blit_char.o: blit_char.s
	$(ASM2OBJ)
$(OBJPATH)set_olp.o: set_olp.s
	$(ASM2OBJ)

#
# 68k jaguar specific C files
#
$(OBJPATH)skunkTimeout.o: skunkTimeout.c
	$(C2OBJ)
$(OBJPATH)wait_blitter_ready.o: wait_blitter_ready.c
	$(C2OBJ)
$(OBJPATH)attach_olp.o: attach_olp.c
	$(C2OBJ)
$(OBJPATH)append_olp.o: append_olp.c
	$(C2OBJ)
$(OBJPATH)restore_ol.o: restore_ol.c
	$(C2OBJ)
$(OBJPATH)console_save_bmp.o: console_save_bmp.c
	$(C2OBJ)
$(OBJPATH)console_show.o: console_show.c
	$(C2OBJ)
$(OBJPATH)console_hide.o: console_hide.c
	$(C2OBJ)
$(OBJPATH)console_hide_startup.o: console_hide_startup.c
	$(C2OBJ)
$(OBJPATH)set_indexed_color.o: set_indexed_color.c
	$(C2OBJ)
$(OBJPATH)libmain.o: libmain.c
	$(C2OBJ)
$(OBJPATH)global_interrupt_handler.o: global_interrupt_handler.c
	$(C2OBJ)
$(OBJPATH)custom_interrupt_handler.o: custom_interrupt_handler.c
	$(C2OBJ)
$(OBJPATH)welcome_message.o: welcome_message.c
	$(C2OBJ)
$(OBJPATH)init_message.o: init_message.c
	$(C2OBJ)
$(OBJPATH)io_open.o: io_open.c
	$(C2OBJ)
$(OBJPATH)jagopen.o: jagopen.c
	$(C2OBJ)
$(OBJPATH)io_close.o: io_close.c
	$(C2OBJ)
$(OBJPATH)jagclose.o: jagclose.c
	$(C2OBJ)
$(OBJPATH)io_seek.o: io_seek.c
	$(C2OBJ)
$(OBJPATH)jagseek.o: jagseek.c
	$(C2OBJ)
$(OBJPATH)io_read.o: io_read.c
	$(C2OBJ)
$(OBJPATH)jagread.o: jagread.c
	$(C2OBJ)
$(OBJPATH)io_write.o: io_write.c
	$(C2OBJ)
$(OBJPATH)jagwrite.o: jagwrite.c
	$(C2OBJ)
$(OBJPATH)putc.o: putc.c
	$(C2OBJ)
$(OBJPATH)read_stick0.o: read_stick0.c
	$(C2OBJ)
$(OBJPATH)read_stick1.o: read_stick1.c
	$(C2OBJ)
$(OBJPATH)audio_mute.o: audio_mute.c
	$(C2OBJ)
$(OBJPATH)dsp.o: dsp.c
	$(C2OBJ)
$(OBJPATH)gpu.o: gpu.c
	$(C2OBJ)
#jaglib-files-end