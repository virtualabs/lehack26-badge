leHACK 2026 Collector Badge
===========================

This project is a small collector badge designed for [leHACK 2026](https://lehack.org) rewarding
those who managed to hack my electronic hat during the event. Since this badge looks nice and required
a few weeks of effort to be designed and assembled, I thought it could be worth it to share the design
files and its original firmware (which was a bit late, actually, and had been finished a few weeks after
the event took place...).

* [design](./design) contains my FreeCAD project file as well as the corresponding STL files
* [electronics](./electronics) contains the final export (Gerber files for JLCPCB) and the schematics. BOM is still missing but should be added pretty soon.
* [firmware](./firmware) contains the source code of the firmware, which relies on a [specific version of TFT_eSPI](https://medium.com/@androidcrypto/getting-started-with-an-esp32-c6-waveshare-lcd-device-with-1-47-inch-st7789-tft-display-07804fdc589a), the settings used are provided in this directory

Design process
--------------

This badge has been designed to use a 1200 mAh 103040 Li-ion battery to keep it alive during at least a full day.
Its assembly requires 4 M3 6mm self-taping screws to lock the two main halves of its shell in place. Two button
caps have been designed in the shell itself, relying on the flexibility of the filament used to print these parts.

!(Final assembly, as seen in FreeCAD)[https://github.com/virtualabs/lehack26-badge/blob/main/images/3d-render-badge-final.png]
!(Exploded assembly, as seen in FreeCAD)[https://github.com/virtualabs/lehack26-badge/blob/main/images/3d-render-pcb-with-shell.png]
!(Main PCB)[https://github.com/virtualabs/lehack26-badge/blob/main/images/badge_pcb_render.jpg]
