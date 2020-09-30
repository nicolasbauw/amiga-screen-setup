/*
	Programmation : C - Ouverture d'un écran avec graphics.library
	Article écrit par Pascal Amiable et extrait d'Amiga News Tech - mars 1991
	http://obligement.free.fr/articles/c_ecrans_graphics.php

	Adapté à VBCC par Tygre, 2020/09/06
    Fixed includes, removed exit(), makefile for cross-compilation by Nicolas BAUW, 2020/09/29
    Cross-compiler setup : https://github.com/nicolasbauw/Amiga-cc
*/

/* ----------------------------------------------------------------- */
/*     Ouverture d'un écran sous graphics...                         */
/*     Auteur Pascal AMIABLE (c) 1991                                */
/* ----------------------------------------------------------------- */

#include <exec/types.h>
#include <proto/graphics.h>
#include <proto/exec.h>
#include <graphics/rastport.h>
#include <graphics/copper.h>
#include <graphics/view.h>
#include <graphics/gels.h>
#include <graphics/regions.h>
#include <graphics/clip.h>
#include <graphics/text.h>
#include <graphics/gfxbase.h>
#include <hardware/dmabits.h>
#include <hardware/custom.h>
#include <hardware/blit.h>

#define PLAN     2          /* Nombre de Bitplane associé à l'écran */
#define COLOR    4          /* Nombre de couleurs disponibles */
#define HORIZONT 640        /* Largeur de l'écran en pixels */
#define VERT     512        /* Hauteur de l'écran en pixels */
#define MODE_V   LACE       /* Mode de visualisation du View (entrelacé) */
#define MODE_VP  HIRES+LACE /* Mode de visualisation ViewPort */
							/* Haute résolution et entrelacé  */

int i;

struct View view;         /* View associé à l'écran */
struct ViewPort viewport; /* ViewPort associé à View */
struct RasInfo rasinfo;   /* Structure RasInfo liée au ViewPort */
struct BitMap bitmap;     /* BitMap lié au ViewPort */
struct RastPort rastport; /* Le RastPort avec lequel on déssine */

struct GfxBase *GfxBase; /* Pointeur sur la graphics.library */

struct View *ecran_sauvegarde; /* Pointeur sur une structure View afin de sauvegarder l'écran courant */

UWORD colortable[COLOR] = { 0x000, 0xf00, 0x0f0, 0x00f };
 /* Table des couleurs, couleur 0 = NOIR, couleur 1 = ROUGE, couleur 2 = VERT, couleur 3 = BLEU */

char *bouton_gauche = (char *)0xbfe001;
 /* Port A du CIA dont le bit correspond au bouton gauche de la souris */

int init(), ouvreecran();
void libere();

/* ----------------------------------------------------------------- */
/*     Programme principal                                           */
/* ----------------------------------------------------------------- */

int main()
{
	if (init()) return 1;
	if (ouvreecran()) return 1;
	Move(&rastport, 100, 100);
	Draw(&rastport, 200, 200);
	while(!((*bouton_gauche & 0x40) - 64)); /* Tant que le bouton gauche de la souris n'est pas enfoncé */
	LoadView(ecran_sauvegarde);             /* Restauration du View sauvegardé */
	libere();
}

/* ----------------------------------------------------------------- */
/*     init() Ouvre la bibliothèque et sauvegarde l'ancien écran     */
/* ----------------------------------------------------------------- */

int init()
{
	if ((GfxBase = (struct GfxBase *)OpenLibrary("graphics.library", 0)) == NULL)
		return(1); /* La bibliothèque graphics ne veut pas s'ouvrir */

	ecran_sauvegarde = GfxBase->ActiView; /* Sauvegarde du View actif */
    return(0);
}

/* ----------------------------------------------------------------- */
/*     ouvreecran() Initialise et ouvre un écran                    */
/* ----------------------------------------------------------------- */

int ouvreecran()
{
	InitView(&view);      /* Initialisation de la structure View */
	InitVPort(&viewport); /* Initialisation de la structure ViewPort */

	view.ViewPort = &viewport; /* On lie le ViewPort au View */
	view.Modes = MODE_V;       /* On indique le mode visualisation du View */
	
	InitBitMap(&bitmap, PLAN, HORIZONT, VERT); /* On initialise la bitmap */
	
	rasinfo.BitMap = &bitmap; /* Liaison de la BitMap avec le RasInfo */
	rasinfo.RxOffset = 0;
	rasinfo.RyOffset = 0;
	rasinfo.Next = NULL;      /* Pas d'autres structures */
	
	viewport.DxOffset = 0; /* On remplit la structure ViewPort */
	viewport.DyOffset = 0; /* cf. l'article ci-dessus */
	viewport.DWidth = HORIZONT;
	viewport.DHeight = VERT;
	viewport.RasInfo = &rasinfo;
	viewport.Modes = MODE_VP;
	viewport.Next = NULL;  /* Pas d'autre ViewPort */

	viewport.ColorMap = (struct ColorMap *)GetColorMap(COLOR);
                        /* Initialisation de la ColorMap du ViewPort */
	LoadRGB4(&viewport, colortable, COLOR);
                       /* Chargement des couleurs dans la ColorMap */

	for(i = 0; i < PLAN; i++) /* Allocation des bitplanes */
	{
		if((bitmap.Planes[i] = (PLANEPTR)AllocRaster(HORIZONT, VERT)) == NULL)
			return(1);
		BltClear((UBYTE *)bitmap.Planes[i], RASSIZE(HORIZONT, VERT), 0);
                                   /* Éffacement de bitplanes au Blitter */
	}
	
	InitRastPort(&rastport);    /* Initialisation du RastPort */
	rastport.BitMap = &bitmap; /* On lie le RastPort à la bitmap dans laquelle on déssine */
	
	MakeVPort(&view, &viewport); /* On crée la zone de visualisation */
	MrgCop(&view);               /* et la CopperList associée */
	LoadView(&view);             /* et on affiche l'écran sur le moniteur */
    return 0;
}

/* ----------------------------------------------------------------- */
/*     libere() Libère la mémoire utilisée                           */
/* ----------------------------------------------------------------- */

void libere()
{
	for(i = 0; i < PLAN; i++) /* On libère la mémoire des bitplanes */
		FreeRaster(bitmap.Planes[i], HORIZONT, VERT);
	
	FreeColorMap(viewport.ColorMap); /* On libère la mémoire de la ColorMap */
	
	FreeVPortCopLists(&viewport); /* Libération de la CopperList */
	
	CloseLibrary((struct Library *)GfxBase); /* Fermeture de la bibliothèque graphics */
}