<CsoundSynthesizer>
<CsOptions>
</CsOptions>
<CsInstruments>

sr = 44100
ksmps = 64
nchnls = 2
0dbfs = 1.0

;GRAIN ENVELOPE WINDOW FUNCTION TABLES:
giwfn1	ftgen	0,  0, 512,  20, 2				        	; HANNING WINDOW

;WAVEFORMS:
gisine		ftgen	0,        0,     4096,         10,    1			        ;SINE WAVE


;Data mapping
gkpbfreq init 10 ;frequency of reading table (Hz)

instr granularReverb
itable = p4 ;which table number (which "pixel" in data)
insnd   = gisine

;Parameter Initialization
kamp init 1.0
kpitch	init 440
kdens   init 1
kaoff  	init 0.0
kpoff  	init 0.0
kgdur   init 0.01
imgdur 	init 0.05
gkgrnd	init 0
gkRvbAmt	init 0.9	            ;Reverb amount
gkRvbSize	init 0.9	            ;Reverb size
printk2 gkpbfreq


aSig grain kamp, kpitch, kdens, kaoff, kpoff, kgdur, insnd, giwfn1, imgdur, 0.0

gaRvbSend	init	0
gaRvbSend = gaRvbSend + (aSig*gkRvbAmt)	;ADD TO REVERB SEND VARIABLE (LEFT CHANNEL)


outs aSig, aSig

endin

instr	reverb	;REVERB
aRvb,aRvb	reverbsc	gaRvbSend, gaRvbSend, gkRvbSize, 10000
                        outs		aRvb, aRvb
                        clear		gaRvbSend
endin

instr 51
	asig oscil 0.1, 440
	azero = 0
	outs asig, azero
endin

</CsInstruments>
<CsScore>
i 51 0 60000
</CsScore>
</CsoundSynthesizer>
<bsbPanel>
 <label>Widgets</label>
 <objectName/>
 <x>1223</x>
 <y>266</y>
 <width>320</width>
 <height>240</height>
 <visible>true</visible>
 <uuid/>
 <bgcolor mode="nobackground">
  <r>255</r>
  <g>255</g>
  <b>255</b>
 </bgcolor>
</bsbPanel>
<bsbPresets>
</bsbPresets>
