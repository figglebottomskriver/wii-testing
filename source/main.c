#include <stdio.h>
#include <stdlib.h>
#include <gccore.h> // libogc core, i'm guessing
#include <wiiuse/wpad.h> // for interact with wiimote

static void *xfb = NULL; // pointer to framebuffer
static GXRModeObj *rmode = NULL; // holds selected video and render settings

int main(int argc, char **argv) { // as far as i can tell this is just the default args people do even if they arent used
    VIDEO_Init();
    WPAD_Init();

    // gets preferred video mode (i think this means 4:3 or 16:9), i think the null is indicating that it uses whatever the system is set as
    rmode = VIDEO_GetPreferredMode(NULL); 
    xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode)); // allocates memory for the display, maybe

    console_init(xfb, 0, 0, rmode->fbWidth, rmode->xfbHeight, rmode->fbWidth*VI_DISPLAY_PIX_SZ); // i don't know what stride means

    VIDEO_Configure(rmode); // sets up video registers with chosen mode ??
    VIDEO_SetNextFramebuffer(xfb); // tells video hardware where  display memory is
    VIDEO_SetBlack(false); // makes display visible
    VIDEO_Flush(); // flushes video register changes to the hardware

    // waits for video setup to complete
    VIDEO_WaitVSync();
    if(rmode->viTVMode & VI_NON_INTERLACE) VIDEO_WaitVSync();

    printf("\x1b[0;0H"); // positions cursor at row 2 column 0
    printf("Very Quality Wiimote Testing Program (only for player one)\n");

    while(1) {
        WPAD_ScanPads(); // reads latest controller states

        u32 pressed = WPAD_ButtonsDown(0); // checks first remote 

        if (pressed) switch (pressed) {
            case WPAD_BUTTON_HOME:
                exit(0);
                break;
            case WPAD_BUTTON_A:
                printf("A Button Pressed\n");
                break; 
        }

        VIDEO_WaitVSync(); // wait for next frame
    }
}
