#include <proto/exec.h>
//#include <proto/dos.h>
//#include <proto/graphics.h>
#include <graphics/gfxbase.h>
#include <graphics/view.h>
//#include <exec/execbase.h>
#include <graphics/gfxmacros.h>
#include <hardware/custom.h>
//#include <hardware/dmabits.h>
//#include <hardware/intbits.h>

// pointer to bitplan data
UBYTE *bitplan1;

struct ExecBase *ExecBase;
//struct DosLibrary *DOSBase;
struct GfxBase *GfxBase;
struct copinit *oldcop;

//backup
static UWORD SystemDMA;

volatile UBYTE *ciaa = (volatile UBYTE *) 0xbfe001;

// Basic copperlist : just resets bitplan pointers at each frame
UWORD __chip clist[] = {
    0x00E2, 0x0000,
    0x00E0, 0x0000,
    0xFFFF, 0xFFFE
};

static void mouse_left(void)
{
    while ((*ciaa & 64) != 0) ;
}

void startup() {
    // Allocating memory (chipram) for bitplans
    UBYTE *bitplan1 = AllocMem(0x2800, MEMF_CHIP);
    ULONG bpl1addr;
    bpl1addr = (ULONG)bitplan1;
    clist[1] = (UWORD)bpl1addr>>16;
    clist[3] = (UWORD)bpl1addr;

    // Saving initial copperlist
	GfxBase = (struct GfxBase*)OpenLibrary("graphics.library", 0);
	oldcop = GfxBase->copinit;
	CloseLibrary((struct Library *)GfxBase);
}

void restore() { 
	
}

int main() {
    startup();
    mouse_left();
    restore();
    return 0;
}