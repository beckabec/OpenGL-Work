***********************************************
***********************************************
*                                             *
*	Vector library & Obj file loader      *
*                                             *
*                                             *
***********************************************
***********************************************

[Vector Library Compile & Usage]

 - Windows (Visual Studio):
	1. In project property dialog, add following line to C/C++->General->Additional Include Directories
		
		.\ilmbase\inc\half, .\ilmbase\inc\ilmthread, .\ilmbase\inc\iex, .\ilmbase\inc\imath

	2. Include corresponding header files, ex.
		#include "imathvec.h"

 - Most of the classes in IMath library are templates. Just include the corresponding header files. In case you need to recompile the library, the code is in .\ilmbase\src\


[Demo Program]

The program shows how to use the objloader to load .obj files into a Mesh structure. The model is simple shaded by some random colors. No lighting is applied.

- cotrol:
	1. Rotation: Mouse Left Button
	2. Scale: Mouse Right Button
	3. Menu: Mouse Middle Button

[Folder Structure]
The demo program will use models from Models.zip we provided. The models folder should be placed in the same directory as this demo program. The folder structure should look as below:
...\
|---MeshDataStructure_VectorLibrary_FileReader\
    |---Codes & Exe
|---Models\
    |---Required\

[Dll Dependency]
Since the exe in this zip file is compiled by Visual Studio 2010, it may require MSVCR100.dll to run it if you don't have Visual Studio 2010 installed. In that case, just recompile the program by your version of Visual Studio, and copy the exe from Release folder.