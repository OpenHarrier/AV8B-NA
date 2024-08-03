# OfficialOpenHarrierModel
This is the official FreeCAD Model for OpenHarrier.

As of this writing, the version of FreeCAD being used is 0.21.2.

DISCLAIMER:  This model (and any Harrier Model, for that matter) is not 100% perfect.  We've attempted to make the model as accurate as possible based on the very few dimensions that we've been able to curate.  Note that in some cases, dimensions are approximate as there have been multiple measurements made by multiple 
people that all differ from each other.  In those cases, we'd attempted to approximate to what "seems" 
to be the most realistic.  Everything else is interpolated based off of the known dimensions.  
Will it be a perfect match for a real Harrier cockpit.  No, but it should be close.  
I will also mention that the Razbam DCS Harrier cockpit has been used for reference and some of the interpolation, 
and it is known that the Razbam cockpit does indeed have some inaccuracies.  
For example, in the Razbam cockpit has the left and right side consoles flat and horizontal with respect to the floor.
This is known to be totally inaccurate.  In the real Harrier, these consoles are sloped back by 10 degrees down towards
the aft of the cockpit.  We've chosen to reflect the real cockpit in this respect and not the Razbam cockpit.

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


FreeCAD addons you need to install in order to open this model without major errors:
    Assembly4
    Curves workbench
    Fasteners Workbench
    freecad.gears workbench
    Render


KNOWN ISSUES:
* The placeholder panels on the left and right consoles are floating on a horizontal plane in the cockpit and are not in the correct position.  These need to be redone and have their origins corrected and tranformed to the correct positions.
* The UFC model is NOT complete and some of the mounting connections between pieces are still being worked out.  The circuit board is a placeholder and has not been designed yet.
* The structures for the left and right consoles are in the process of being refactored.  The blue members are placeholders for parts that will replicate the outside fuselage curves of the Harrier.  That is a work in progress.
* The canopy ring is twice as thick as the real canopy ring - this was done to beef it up since mine is made from MDF and I wanted to protect it against being snapped. We will design 2 different versions of the canopy ring - one that can be fabricated to accurte dimensions from metal, and one that can be made via wood.
* There might be a minor part on the front of the ejection seat that is missing and needs to be remade.  This is a minor part and has no structural purpose.
* You may see some errors in the FreeCAD report view about "freecad.gears.features.ViewAProviderGear".  I believe that's due to some incompatibities in an older version of a Gears workbench I had used when working on the standby compass.  I plan to redo all of that, which will fix those issues. 

If you want to contribute to the OpenHarrier project, please reach out
to Jason at skystream172@gmail.com

