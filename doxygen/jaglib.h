/**
 * @mainpage The ATARI Jaguar console library
 * @author Tobias Arnold
 * @date 2016-2017
 * @file jaglib.h
 * @version 0.1
 * @brief ATARI Jaguar console library
 *
 * The jaglib is part of the ATARI Jaguar target in the [vbcc](http://sun.hasenbraten.de/vbcc/) package.
 * The library is designed to act in conjunction with the vclib, so it is easy to take first steps in ATARI Jaguar programming.
 *
 * Check the [github repository](https://github.com/toarnold/jaglib-demo) for demo code. 
 */
#ifndef _jaglib_h
#define _jaglib_h 1

#include <stdint.h>
#include <stdlib.h>

/* Blitter specific functions */ 
/**
 *  @brief Blocks the CPU until the blitter is ready.
 */
void jag_wait_blitter_ready();
/**
 *  @brief Fills a memory area with a byte-value using the blitter.
 *
 *  This function returns immediately if the blitter free.
 *  This function will block until the blitter is available.
 *  Note: Check if blitter is ready before accessing the dest area.
 *
 *  @param dest		pointer to the destination area
 *  @param repcount	count of memory areas
 *  @param count	size in bytes of the area
 *  @param value	the value to fill in
 *	@see jag_wait_blitter_ready()
 */
void jag_memset8(void *dest,uint16_t repcount,uint16_t count,uint8_t value);
/**
 *  @brief Fills a memory area with a 32bit-value using the blitter.
 *
 *  This function returns immediately if the blitter free.
 *  This function will block until the blitter is available.
 *  Note: Check if blitter is ready before accessing the dest area.
 *
 *  @param dest		pointer to the destination area
 *  @param repcount	count of memory areas
 *  @param count	size in longwords of the area
 *  @param value	the value to fill in
 *	@see jag_wait_blitter_ready()
 */
void jag_memset32(void *dest,uint16_t repcount,uint16_t count,uint32_t value);
/**
 *  @brief Copies memory areas, which are muliples of 16bit, using the blitter.
 *
 *  This function returns immediately if the blitter free.
 *  This function will block until the blitter is available.
 *  Note: Check if blitter is ready before accessing the dest area.
 *
 *  @param dest		pointer to the destination area (Must be phrase aligned)
 *  @param src		pointer to the source area (Must be phrase aligned)
 *  @param repcount	count of memory areas
 *  @param wordcount size in words of the area
 *	@see jag_wait_blitter_ready()
 */
void jag_memcpy16p(void *dest,const void *src,uint16_t repcount,uint16_t wordcount);
/**
 *  @brief Copies memory areas, which are muliples of 32bit, using the blitter.
 *
 *  This function returns immediately if the blitter free.
 *  This function will block until the blitter is available.
 *  Note: Check if blitter is ready before accessing the dest area.
 *
 *  @param dest		pointer to the destination area (Must be phrase aligned)
 *  @param src		pointer to the source area (Must be phrase aligned)
 *  @param repcount	count of memory areas
 *  @param longcount size in longwords of the area
 *	@see jag_wait_blitter_ready()
 */
void jag_memcpy32p(void *dest,const void *src,uint16_t repcount,uint16_t longcount);

/* Object list specific functions */
/**
 *  @brief Set a new object list.
 *
 *  This function returns immediately.
 *  The pointer will be set during the next vertical blank interrupt.
 *
 *  @param ol		pointer to new object list
 */
void jag_set_olp(__reg("d0") const void *ol);
/**
 *  @brief Branch to the given object list.
 *
 *  The branch is taken before the console bitmap. So if you don't branch back, the console will be hidden.
 *  This function returns immediately.
 *  The pointer will be set during the next vertical blank interrupt.
 *
 *  @param olp		pointer to new object list
 */
void jag_attach_olp(const void* olp);
/**
 *  @brief Branch to the given object list.
 *
 *  The branch is taken after the console bitmap. So the list will be displayed above the console bitmap.
 *  This function returns immediately.
 *  The pointer will be set during the next vertical blank interrupt.
 *
 *  @param olp		pointer to new object list
 */
void jag_append_olp(const void* olp);
/**
 *  @brief Restore the intial object list.
 *
 *  This function returns immediately.
 *  The pointer will restored during the next vertical blank interrupt.
 */
void jag_restore_ol();

/* Console specific functions */
/**
 *  @brief Clear the console bitmap.
 *
 *  The cursor will be set to position (0,0).
 */
void jag_console_clear();
/**
 *  @brief Save the current console bitmap.
 *
 *  This functions is only valid if a SkunkBoard is present and attached.
 *
 *  @param fname	A valid file name. No path informations. If the file exists, it will be overwritten.
 */
void jag_console_save_bmp(const char *fname);
/**
 *  @brief Displays the console bitmap.
 */
void jag_console_show();
/**
 *  @brief Hides the console bitmap.
 */
void jag_console_hide();
/**
 *  @brief Set the cursor to the given position
 *
 *	Set the cursor to the position (x,y).
 *
 *  @param x	The x coordinate to set.
 *  @param y	The y coordinate to set.
 */
void jag_console_set_cursor(uint16_t x,uint8_t y);
/**
 *  @brief Display init message
 *
 *	Override this function to get a custom init message (or supress the standard init message)
 */
void jag_init_message();
/**
 *  @brief Display welcome message
 *
 *	Override this function to get a custom welcome message (or supress the standard welcome message)
 */
void jag_welcome_message();

/* Enter small debugger */
/**
 *  @brief Enter a small debugger
 *
 *	Trigger the debugger manually. The library occupies all 68000 exception vectors. If an exception occurs the debugger code takes place.
 *	If a SkunkBoard is present, the output will be redirected.<br>Note: The debugger can't be exited.<br>
 * m <addr> [lines] : memory dump<br>
 * p <addr> : print offset<br>
 * r : register dump<br>
 * x : exit (detach skunkboard)
 */
void jag_debug();

/* Video specific functions */
/** @brief The console width in pixel */
#define CONSOLE_BMP_WIDTH	320
/** @brief The console height in pixel */
#define CONSOLE_BMP_HEIGHT	200
/// @cond
extern uint16_t jag_hdb;
extern uint16_t jag_hde;
extern uint16_t jag_vdb;
extern uint16_t jag_vde;
extern uint16_t jag_width;
extern uint16_t jag_height;
/// @endcond

/**
 *  @brief Blocks the CPU until the next vertical blank interrupt occurs.
 */
void jag_wait_vbl();

/**
 * @brief Handle CPU Interrupts.
 * @return The flag to confirm the handled interrupt. e.g. return C_PITCLR
 *
 * Check INT1 to detect which interrupt type happens.
 */
uint16_t jag_custom_interrupt_handler();

/**
 * @brief Modifies the INT1 register
 *
 * Don't modify INT1 directly because the internal interrupt handler needs this mask, too.
 */
void jag_set_cpu_int_mask(uint16_t mask);

/** @brief Converts 24bit RGB Color to 16bit RGB value  @hideinitializer */
#define toRgb16(r,g,b) ((((r)&0xf8) << 8) | (((b)&0xf8) << 3) | (((g)&0xfc)>>2))
/**
 *  @brief Set the index color
 *
 *	Set an indexed color in the CLUT table
 *
 *  @param index	Index in the CLUT table
 *  @param color	Color value to be set.
 */
void jag_set_indexed_color(uint16_t index,uint16_t color);
/** @brief pointer to the memory area used by the console bitmap (Size: CONSOLE_BMP_HEIGHT*CONSOLE_BMP_HEIGHT bytes). */
extern uint8_t *jag_vidmem;
/** @brief pointer to the standard object list. */
extern uint64_t *jag_listbuf;

/* Declaration for custom io function */
/**
 *  @brief Custom file open implementation
 *
 *	Define this function in the target code to prevent the default implementation to execute, or define custom file handling code.
 *
 *  @param name	The filename provided by fopen
 *  @param mode	The access mode provided by fopen
 *	@return -1: Executes the base implementation, >0: the file handle, <-1: an error code. Never use the values 0,1,2 (std file handles) or 3 (SKunkBoard file handle).
 */
int jag_io_open(const char *name,const char *mode);
/**
 *  @brief Custom file close implementation
 *
 *	Define this function in the target code to prevent the default implementation to execute, or define custom file handling code.
 *
 *  @param handle	The file handle provided by fopen
 *	@return 0: Executes the base implementation, <>0: the success or error code.
 */
int jag_io_close(int handle);
/**
 *  @brief Custom file seek implementation
 *
 *	Define this function in the target code to prevent the default implementation to execute, or define custom file handling code.
 *
 *  @param handle	The file handle provided by fopen
 *  @param offset	Number of bytes to offset from origin
 *  @param origin	The origin from fseek
 *	@return 0: Success, <>0: the error code
 */
long jag_io_seek(int handle,long offset,int origin);
/**
 *  @brief Custom file read implementation
 *
 *	Define this function in the target code to define custom file seeking code. There is no base definition, because the jaglib doesn't support seeking.
 *
 *  @param handle	The file handle provided by fopen
 *  @param buffer	The buffer to write the incomming values
 *  @param length	The size of the buffer
 *	@return -1: End of File, >0: count of values read, <-1: Custom error code
 */
size_t jag_io_read(int handle,char *buffer,size_t length);
/**
 *  @brief Custom file write implementation
 *
 *	Define this function in the target code to prevent the default implementation to execute, or define custom file handling code.
 *
 *  @param handle	The file handle provided by fopen
 *  @param buffer	The buffer containing the chars to write
 *  @param length	The size of the buffer
 *	@return The count of written chars or an custom error code. -1: Indicates to call the base implementation.
 */
size_t jag_io_write(int handle,const char *buffer, size_t length);

/* joystick functions */
/**
 *  @brief Argument for jag_read_stick0 or jag_read_stick1
 *
 *	Force to read the joypad the 'A' and the 'Pause' button only. (Faster)
 *	@see jag_read_stick0
 *	@see jag_read_stick1
 */
#define STICK_READ_DIRECTIONS_A_ONLY 1
/**
 *  @brief Argument for jag_read_stick0 or jag_read_stick1
 *
 *	Force to read all joystick states. (Slower)
 *	@see jag_read_stick0
 *	@see jag_read_stick1
 */
#define STICK_READ_ALL 0

#define STICK_OPTION	(1<<0) /**< @brief Flag indicating joystick 'Option' is pressed @hideinitializer */
#define STICK_HASH		(1<<1) /**< @brief Flag indicating joystick '#' is pressed @hideinitializer */
#define STICK_9			(1<<2) /**< @brief Flag indicating joystick '9' is pressed @hideinitializer */
#define STICK_6			(1<<3) /**< @brief Flag indicating joystick '6' is pressed @hideinitializer */
#define STICK_3			(1<<4) /**< @brief Flag indicating joystick '3' is pressed @hideinitializer */
#define STICK_C			(1<<5) /**< @brief Flag indicating joystick 'C' is pressed @hideinitializer */
#define STICK_0			(1<<6) /**< @brief Flag indicating joystick '0' is pressed @hideinitializer */
#define STICK_8			(1<<7) /**< @brief Flag indicating joystick '8' is pressed @hideinitializer */
#define STICK_5			(1<<8) /**< @brief Flag indicating joystick '5' is pressed @hideinitializer */
#define STICK_2			(1<<9) /**< @brief Flag indicating joystick '2' is pressed @hideinitializer */
#define STICK_B			(1<<10) /**< @brief Flag indicating joystick 'B' is pressed @hideinitializer */
#define STICK_STAR		(1<<11) /**< @brief Flag indicating joystick '*' is pressed @hideinitializer */
#define STICK_7			(1<<12) /**< @brief Flag indicating joystick '7' is pressed @hideinitializer */
#define STICK_4			(1<<13) /**< @brief Flag indicating joystick '4' is pressed @hideinitializer */
#define STICK_1			(1<<14) /**< @brief Flag indicating joystick '1' is pressed @hideinitializer */
#define STICK_PAUSE		(1<<15) /**< @brief Flag indicating joystick 'Pause' is pressed @hideinitializer */
#define STICK_A			(1<<16) /**< @brief Flag indicating joystick 'A' is pressed @hideinitializer */
#define STICK_UP		(1<<17) /**< @brief Flag indicating joypad 'Up' is pressed @hideinitializer */
#define STICK_DOWN		(1<<18) /**< @brief Flag indicating joypad 'Down' is pressed @hideinitializer */
#define STICK_LEFT		(1<<19) /**< @brief Flag indicating joypad 'Left' is pressed @hideinitializer */
#define STICK_RIGHT		(1<<20) /**< @brief Flag indicating joypad 'Right' is pressed @hideinitializer */
/**
 *  @brief Read the value of joystick0
 *
 *  This functions takes care about the mute flag.
 *
 *	@param	readdirectionsonly	Determine whether to read all states or a quick subset only.
 *	@return	bitflags representing the joystick buttons
 *	@see STICK_READ_DIRECTIONS_A_ONLY
 *	@see STICK_READ_ALL
 *  @see jag_audio_mute
 */
uint32_t jag_read_stick0(uint16_t readdirectionsonly);
/**
 *  @brief Read the value of joystick1
 *
 *  This functions takes care about the mute flag.
 *
 *	@param readdirectionsonly	Determine whether to read all states or a quick subset only.
 *	@return	bitflags representing the joystick buttons
 *	@see STICK_READ_DIRECTIONS_A_ONLY
 *	@see STICK_READ_ALL
 *  @see jag_audio_mute
 */
uint32_t jag_read_stick1(uint16_t readdirectionsonly);

/**
 *  @brief Mute or un-mute the audio subsystem. The joystick command respect this flag, because the mute flag is part of the joystick registers.
 *
 *	@param mute	A zero un-mute the audio subsystem. All other values will mute the audio subsystem.
 *	@see jag_read_stick0
 *	@see jag_read_stick0
 */
void jag_audio_mute(uint16_t mute);

/* dsp functions */
/**
 *  @brief Loads a subroutine into dsp memory
 *
 *	@param loadadr	A load address in dsp memory space (Must be phrase aligned)
 *	@param codestartadr	The source address to blit from (Must be phrase aligned)
 *	@param count Count of bytes to copy
 */
void jag_dsp_load(void *loadadr,const void *codestartadr, uint16_t count);
/**
 *  @brief Test if the dsp is in use
 *
 *	@return 0: dsp is not in use, <>0: dsp is in use
 */
uint8_t jag_dsp_is_running();
/**
 *  @brief Starts a dsp routine
 *
 *	@param dspstartadr The start address in dsp memory space.
 *	@param addFlags	Additional flags for the D_CTRL register. RISCGO will always be set.
 *	@return 0: dsp cannot be started because the dsp is currently running. 1: Otherwise
 */
uint8_t jag_dsp_go(const void *dspstartadr, uint16_t addFlags);
/**
 *  @brief Blocks the cpu until the dsp is ready
 */
void jag_dsp_wait();

/* gpu functions */
/**
 *  @brief Loads a subroutine into gpu memory
 *
 *	@param loadadr	A load address in gpu memory space (Must be phrase aligned)
 *	@param codestartadr	The source address to blit from (Must be phrase aligned)
 *	@param count Count of bytes to copy
 */
void jag_gpu_load(void *loadadr,const void *codestartadr, uint16_t count);
/**
 *  @brief Test if the gpu is in use
 *
 *	@return 0: gpu is not in use, <>0: gpu is in use
 */
uint8_t jag_gpu_is_running();
/**
 *  @brief Starts a gpu routine
 *
 *	@param gpustartadr The start address in dsp memory space.
 *	@param addFlags	Additional flags for the G_CTRL register. RISCGO will always be set.
 *	@return 0: dsp cannot be started because the dsp is currently running. 1: Otherwise
 */
uint8_t jag_gpu_go(const void *gpustartadr, uint16_t addFlags);
/**
 *  @brief Blocks the cpu until the gpu is ready
 */
void jag_gpu_wait();

/* SkunkBoard specific functions*/
/// @cond
void skunkRESET();
void skunkCONSOLEWRITE(__reg("a0") char *txt);
void skunkCONSOLECLOSE();
__reg("a0") char *skunkCONSOLEREAD(__reg("a0") char *buffer,__reg("d0") uint16_t maxbytes); // maxbytes max 4064
void skunkNOP();
void skunkFILEOPEN(__reg("a0") const char *filename,__reg("d0") uint8_t mode); // mode=0 (write), move=1 (read)
void skunkFILEWRITE(__reg("a0") const uint8_t *data,__reg("d0")  uint16_t count); // count max. 4060, even
__reg("d0") size_t skunkFILEREAD(__reg("a0") char *buffer,__reg("d0") uint16_t maxbytes); // maxbytes max 4064
void skunkFILECLOSE();
/// @endcond
/**
 *  @brief Indicate if a SkunkBoard is present and attached.
 *
 *	A values <>0 indicates that a SkunkBoard is present.
 *
 */
extern uint32_t skunkConsoleUp;
/**
 *  @brief Indicate whether to test if a SkunkBoard is present.
 *	Disable this test if a SkunkBoard isn't needed. This will speed up the boot process.
 *
 *	<>0: Test, 0: don't test.
 *
 */
extern uint16_t jag_console_hide_startup;

#define JAG_CONSOLE_BACKGROUND_COLORINDEX	253 /**< @brief Colorindex for the console background color @hideinitializer */
#define JAG_CONSOLE_BACKGROUND_COLORINDEX32	0xfdfdfdfd /**< @brief Colorindex for the console background color as 32bit value. Can be used in jag_memset32 @hideinitializer */
#define JAG_CONSOLE_STDOUT_COLORINDEX		254 /**< @brief Colorindex for the console stdout color @hideinitializer */
#define JAG_CONSOLE_STDERR_COLORINDEX		255 /**< @brief Colorindex for the console stderr color @hideinitializer */

/**
 *  @brief Reads the 68000 status register
 *
 *	@return	The current SR value
 */
uint16_t jag_get_sr() =	"\tmove\tsr,d0";
/**
 *  @brief Sets the 68000 status register
 *
 *	@return	 sr new SR value to set.
 */
void jag_set_sr(__reg("d0") uint16_t sr) = "\tmove\td0,sr";

#pragma dontwarn 51
/**
 *  @brief Branch object which can be placed in the object list.
 *
 *  See the common ATARI Jaguar documentation for details
 */
typedef struct
{
	uint64_t reserved0 : 21; /**< @brief Placeholder to align data */
	uint64_t link : 19; /**< @brief This defines the address of the next object if the branch is taken.  */
	uint64_t reserved1 : 7; /**< @brief Placeholder to align data */
	uint64_t cc : 3; /**< @brief These bits specify what condition is used to determine whether to branch */
	uint64_t ypos : 11; /**< @brief This value may be used to determine whether the LINK address is used */
	uint64_t type : 3; /**< @brief Branch object is type three */
} op_branch_object;

/**
 *  @brief Bitmap object which can be placed in the object list.
 *
 *  See the common ATARI Jaguar documentation for details
 */
typedef struct
{
	 /** Phrase 0 object */
	struct {
		uint64_t data : 21; /**< @brief This defines where the pixel data can be found. */
		uint64_t link : 19; /**< @brief This defines the address of the next object. */
		uint64_t height : 10; /**< @brief This field gives the number of data lines in the object. */
		uint64_t ypos : 11; /**< @brief This field gives the value in the vertical counter (in half lines) for the first (top) line of the object. */
		uint64_t type : 3; /**< @brief Bit mapped object is type zero */
	} p0;
	 /** Phrase 1 object */
	struct {
		uint64_t reserved : 9; /**< @brief Placeholder to align data */
		uint64_t firstpix : 6; /**< @brief This field identifies the first pixel to be displayed. */
		uint64_t release : 1; /**< @brief This bit forces the Object Processor to release the bus between data fetches. */
		uint64_t trans : 1; /**< @brief Flag to make logical colour zero and reserved physical colours transparent. */
		uint64_t rmw : 1; /**< @brief Flag to add object to data in line buffer. */
		uint64_t reflect : 1; /**< @brief Flag to draw object from right to left */
		uint64_t index : 7; /**< @brief For images with 1 to 4 bits/pixel the top 7 to 4 bits of the index provide the most significant bits of the palette address. */
		uint64_t iwidth : 10; /**< @brief This is the image width in phrases (must be non zero), and may be used for clipping. */
		uint64_t dwidth : 10; /**< @brief This is the data width in phrases */
		uint64_t pitch : 3; /**< @brief This value defines how much data, embedded in the image data, must be skipped. */
		uint64_t depth : 3; /**< @brief This defines the number of bits per pixel */
		uint64_t xpos : 12; /**< @brief This defines the X position of the first pixel to be plotted. */
	} p1;
} op_bmp_object;

/**
 *  @brief Stop object which can be placed in the object list.
 *
 *  See the common ATARI Jaguar documentation for details
 */
typedef struct {
	uint64_t data1 : 32; /**< @brief These bits may be used by the CPU interrupt service routine. */
	uint64_t data2 : 28; /**< @brief These bits may be used by the CPU interrupt service routine. */
	uint64_t int_flag : 1; /**< @brief Force an interrupt. */
	uint64_t type : 3; /**< @brief Stop object is type four. */
} op_stop_object;

/**
 *  @brief Scaled bitmap object which can be placed in the object list.
 *
 *  Note: This object must be qphrase aligned.
 *  See the common ATARI Jaguar documentation for details
 */
typedef struct
{
	 /** Phrase 0 object */
	struct {
		uint64_t data : 21; /**< @brief This defines where the pixel data can be found. */
		uint64_t link : 19; /**< @brief This defines the address of the next object. */
		uint64_t height : 10; /**< @brief This field gives the number of data lines in the object. */
		uint64_t ypos : 11; /**< @brief This field gives the value in the vertical counter (in half lines) for the first (top) line of the object. */
		uint64_t type : 3; /**< @brief Bit scaled mapped object is type one */
	} p0;
	 /** Phrase 1 object */
	struct {
		uint64_t reserved : 9; /**< @brief Placeholder to align data */
		uint64_t firstpix : 6; /**< @brief This field identifies the first pixel to be displayed. */
		uint64_t release : 1; /**< @brief This bit forces the Object Processor to release the bus between data fetches. */
		uint64_t trans : 1; /**< @brief Flag to make logical colour zero and reserved physical colours transparent. */
		uint64_t rmw : 1; /**< @brief Flag to add object to data in line buffer. */
		uint64_t reflect : 1; /**< @brief Flag to draw object from right to left */
		uint64_t index : 7; /**< @brief For images with 1 to 4 bits/pixel the top 7 to 4 bits of the index provide the most significant bits of the palette address. */
		uint64_t iwidth : 10; /**< @brief This is the image width in phrases (must be non zero), and may be used for clipping. */
		uint64_t dwidth : 10; /**< @brief This is the data width in phrases */
		uint64_t pitch : 3; /**< @brief This value defines how much data, embedded in the image data, must be skipped. */
		uint64_t depth : 3; /**< @brief This defines the number of bits per pixel */
		uint64_t xpos : 12; /**< @brief This defines the X position of the first pixel to be plotted. */
	} p1;
	/** Phrase 2 Object */
	struct {
		uint64_t reserved2 : 40; /**< @brief Placeholder to align data */
		uint64_t remainder : 8; /**< @brief This eight bit field contains a three bit integer part and a five bit fractional part.  */
		uint64_t vscale : 8;/**< @brief This eight bit field contains a three bit integer part and a five bit fractional part. */
		uint64_t hscale : 8;/**< @brief This eight bit field contains a three bit integer part and a five bit fractional part. */
	} p2;
	/** Phrase 3 Object */
	struct {
		uint64_t unused : 64; /**< @brief Placeholder to align data */
	} p3;
} op_bmp_object_scaled;
#pragma popwarn

/** @brief pointer to console bitmap list object. */
extern op_bmp_object *jag_console_bmp;
/** @brief pointer to branch object before the console bitmap list object. 
 *
 * If the console bitmap is visible the branch points to then console bitmap object.
 * The branch points to a stop object otherwise.
 */
extern op_branch_object *jag_logical_root;

#endif
