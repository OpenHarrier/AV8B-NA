# OfficialOpenHarrierModel
This is the official FreeCAD Model for OpenHarrier.

As of this writing, the version of FreeCAD being used is 0.21.2.

If you want to load the entire model with all of the sub-assemblies,
you should load the latest AV8B-NA-YYYY-MM-DD.FCStd file, which will load
all of the files so that they can be shown as a single model.

Note that some parts might be made in tools other than FreeCAD, but are
imported into the FreeCAD assembly.  In order to modify those parts, you
may need to utilize other tools to modify them, then export the part into 
a format that can be imported into FreeCAD, such as a STEP or .stp file.

Certified Non-FreeCAD parts will be kept in the CertifiedNonFreeCADParts folder to keep them separated
from the FreeCAD-native portions of the model. Certified means that the parts are known to fit with 
the official FreeCAD model.  This is to help prevent a situation where parts designed by others might not
have the correct scaling or dimensions that allow it to be compatible with other parts from the official model.
Also be aware that some non-FreeCAD parts may be replaced in the future with 
native-designed FreeCAD parts at a later date.

KNOWN ISSUES:
* The placeholder panels on the left and right consoles are floating on a horizontal plane in the cockpit and are not in the correct position.  These need to be redone and have their origins corrected and tranformed to the correct positions.
* The UFC model is NOT complete and some of the mounting connections between pieces are still being worked out.  The circuit board is a placeholder and has not been designed yet.
* The structures for the left and right consoles are in the process of being refactored.  The blue members are placeholders for parts that will replicate the outside fuselage curves of the Harrier.  That is a work in progress.
* The canopy ring is twice as thick as the real canopy ring - this was done to beef it up since mine is made from MDF and I wanted to protect it against being snapped. We will design 2 different versions of the canopy ring - one that can be fabricated to accurte dimensions from metal, and one that can be made via wood.
* There might be a minor part on the front of the ejection seat that is missing and needs to be remade.  This is a minor part and has no structural purpose.

If you want to contribute to the OpenHarrier project, please reach out
to Jason at skystream172@gmail.com

