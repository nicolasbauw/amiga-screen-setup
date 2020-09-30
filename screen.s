;APS00000025000000250000002500000025000000250000002500000025000000250000002500000025
;initialise un ecran en 320*200*1BP

custom = $dff000
bplcon0 = $0100
bplcon1 = $0102
bpl1mod = $0108
bpl2mod = $010A
ddfstrt = $0092
ddfstop	= $0094
diwstrt = $008E
diwstop	= $0090
color00 = $0180
color01 = $0182
dmaconr = $0002
dmacon  = $0096
bpl1pth = $00e0
cop1lc	= $0080
copjmp1	= $0088

	move.l	4,a6
	jsr	-132(a6)		;forbid

	move.l	#$3000,d0		
	move.l	#$10002,d1
	jsr	-198(a6)		;Alloue 1 bplan plus un espace	
	move.l	d0,bitplan		;pour la copperlist en chip
	beq	end			;quitte si allocation echoue

	lea	gfxname,a1
	clr.l	d0
	jsr	-552(a6)		;ouvre graphics.library
	beq	end			;quitte si echec
	move.l	d0,a1
	move.l	38(a1),oldcop		;sauve copperlist systeme
	jsr	-414(a6)		;ferme graphics.library
	
	move.l	bitplan,a1
	move.l	a1,d0
	add.l	#$2900,a1		;adresse copperlist dans a1
	move.l	a1,a2			;sauvegarde dans a2
	move.w	#$00E2,(a1)+		;mise en place de la copperlist
	move.w	d0,(a1)+
	swap	d0
	move.w	#$00E0,(a1)+
	move.w	d0,(a1)+
	move.l	#$FFFFFFFE,(a1)		;fin de generation copperlist

	lea	custom,a0		;adresse de base du chipset
	move.w	dmaconr(a0),d0
	ori.w	#$8000,d0		;SET/CLR a 1
	move.w	d0,olddma		;sauvegarde registre de DMA
	move.w	#$7FFF,dmacon(a0)
	move.w  #$1000,bplcon0(a0)	;1 bitplan en basse resolution
	move.w  #$0000,bplcon1(a0) 
	move.w  #$0038,ddfstrt(a0)
	move.w	#$00D0,ddfstop(a0)
	move.w  #$2C81,diwstrt(a0)
	move.w	#$2CC1,diwstop(a0)
	move.w  #$0000,bpl1mod(a0)
	move.w  #$0000,color00(a0)	;fond noir
	move.w  #$0fff,color01(a0)	;couleur blanche
	move.l	a2,cop1lc(a0)		;add clist dans reg. addr. cop.
	clr.w	copjmp1(a0)		;mise en route copper
	move.w  #$8380,dmacon(a0)	;dma playfield et copper enabled

	bsr	mouse
	
	move.w	olddma,dmacon(a0)	;restore de l'ancienne DMA
	move.l	oldcop,cop1lc(a0)	;restore la copper list systeme
	
end:	move.l	bitplan,a1
	move.l	#$3000,d0
	jsr	-210(a6)		;freemem
	jsr	-138(a6)		;permit
	clr.l	d0			;code retour
	rts

mouse:	btst	#6,$bfe001
	bne	mouse
	rts
	
bitplan	dc.l	0
oldcop	dc.l	0
olddma	dc.w	0
gfxname	dc.b	"graphics.library",0
