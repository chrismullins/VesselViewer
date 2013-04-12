VesselViewer
============

A slight modification of the VTK particle viewer example, to serve as a basis for viewing vessel tortuosity.

In order to compile, configure with the option VTK_DIR to link against VTK.  

`$ git clone git://github.com/chrismullins/VesselViewer.git`

`$ mkdir VV-build && cd VV-build`

`$ cmake -DVTK_DIR=/path/to/VTK-build /path/to/VesselViewer`

`$ make`

`$ ./VesselViewer /path/to/704withTort.tre`
