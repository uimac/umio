# umio

3D file converter/library.

formats
* import: .bos
* import with Autodesk FBX SDK: + .fbx / .dae / .obj / .3ds
* export: .bos
* export with Autodesk FBX SDK: + .fbx

dependency
 * messagepack for bos
 * (optional) pybind11 for pyd (Python DLL)
 * (optional) v8pp for node (node.js native module)
 * (optional) Autodesk FBX SDK for FBX

compiler
 * Visual Studio 2015

license
* MIT-GPLv3 dual license.
* Building with FBXSDK cannot apply GPL. Please see FBXSDK EULA. In this case, Please only use umio as MIT license.
