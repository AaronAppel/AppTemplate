
rem #TODO Loop over every directory instead of manually entering libraries

cd bgfx
del *.vcxproj
del *.user
cd ../

pushd bimg
del *.vcxproj
del *.user
popd

pushd bx
del *.vcxproj
del *.user
popd
