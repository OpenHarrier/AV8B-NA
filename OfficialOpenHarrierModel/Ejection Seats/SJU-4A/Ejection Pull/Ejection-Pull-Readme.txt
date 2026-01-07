OpenHarrier SJU-4A ejection pull mechanism

Consists of 3 parts that are 3D printed (or machined):
Ejection-Pull-Base
Ejection-Pull-Lid
Ejection-Pull-Slider

Additional Parts needed:
(2) M5x45mm threaded bolts
(2) M5x6mmx7mm brass threaded inserts
(2) .7mm x 7mm x 20mm springs 
(1) DAOKI Micro Switch AC 1A 125V 3Pin SPDT NO+NC Hinge (or similar)
(6) M3x16mm self-tapping screws (for lid)
(4) M3x20mm self-tapping screws (for mounting to assembly to wood) or use M3 bolts depending on your application
(2) M1.8x12mm self-tapping screws to mount switch 
(1) 3/16" steel braided cable, 1'8" to 2' long (cut to desired size)
(1) 3/16" steel cable clamp.  I used a screw clamp, but you could also use a crimping clamp, which might be easier to use but harder to remove.  Make sure it fits inside the open area inside the Ejection-Pull-Slider and that it does not interfere with the switch.
(1) Silicon or rubber or other material hose with 1/2" outer diameter and 9mm or so inner diameter, cut to about 25mm shorter than the steel cable.
	I used uxcell Silicone Tubing, 3/8"(9mm) ID x 1/2"(12mm) OD 3.3ft Silicone Rubber Tube Air Hose Water Pipe for Pump Transfer Yellow
	https://www.amazon.com/dp/B085N25ZFR?ref=nb_sb_ss_w_as-reorder_k0_1_18&amp=&crid=3ARFDMUIECAE6&amp=&sprefix=yellow+rubber+hose



Assembly Instructions:
1. With the Ejection-Pull-Base facing upside down, install the brass threaded inserts into the recessed holes on the bottom of the Ejection-Pull-Base using a soldering iron.  Do not push them too far.  The top of the brass threaded inserts should be flush with the recessed hole (not the bottom of the Ejection-Pull-Base).
2. Solder 2 wires to the switch, one to the middle terminal, and the other to the NO (normally open) side of the switch.  No wire is needed on the NC side.
3. Mount switch with the leaf spring mounted towards the bottom of the Ejection-Pull-Base using the (2) 1.8mm self-tapping screws.  
4. Feed the wires through the large hole in the back of the Ejection-Pull-Base
5. Set the Ejection-Pull-Slider into the Ejection-Pull-Base and side it back and forth to confirm it moves freely and pushes the leaf on the switch.
6. Screw the two M5 bolts through the bottom of the Ejection-Pull-Base only enough that so that that the bolts protrude all the way through the holes on the Ejection-Pull-Slider + about 5mm further.
7. Feed a spring onto each M5 bolt that is protruding from the bottom of the Ejection-Pull-Slider, then continuing feeding the M5 bolts all the way through until heads of the bolts are flush with the bottom of the Ejection-Pull-Base.
8. Test the Ejection-Pull-Slider by pulling upward on it and releasing, making sure it springs back to the return position.
9. Cut the steel cable to size.  You can adjust the length to your liking.  I don't know the actual length of the real unit, so I'm just guessing.  After you cut it, wrap each end tightly with electrical tape no more than 2 times around.  This keeps the steel cable from unraveling later, which will be a big problem if it does.  Trust me, I have bloody fingers because I didn't do this!
10. Cut whatever material you are using to be the hose to be slightly shorter than the steel cable.  It should be long enough to fit into the 2 larger holes at the top of the Ejection-Pull-Slider, with the steel cable continuing on into the open area in the middle of the Ejection-Pull-Slider.
11. Feed both ends of the steel cable (with the outer hose material on it) through the outer and inner holes in the top of the Ejection-Pull-Slider so that the steel cable sticks out all the way to the inside bottom of the Ejection-Pull-Slider.
12. Crimp or screw (depending on clamp) the clamp so that the steel cable cannot be pulled out of the holes.  Make sure the clamp does not interfere with the switch.
13. Test the pull by pulling on handle, making sure the Ejection-Pull-Slider slides freely when pulled and springs back to the return position when let go.  Make sure the leaf spring gets pully depressed when you pull on the handle.
14. Screw the Ejection-Pull-Lid to the Ejection-Pull-Base using the (6) M3x16mm self-tapping screws.
15. Retest to make sure the ejection handle works freely.
16. Mount the ejection pull mechanism to your seat.
17. Wire the ejection pull mechanism to your microcontroller of choice.

Arduino sketch provided as an example.  Modify for your own needs.



