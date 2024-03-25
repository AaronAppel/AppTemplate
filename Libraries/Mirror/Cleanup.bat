
del *.dll

del /Q /F /S /AH .vs
del /Q /F /S x64
rmdir /Q /S ".vs"
rmdir /Q /S x64

del /Q /F /S /AH Debug
rmdir /Q /S Debug

del /Q /F /S /AH Release
rmdir /Q /S Release
