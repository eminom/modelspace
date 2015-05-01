The quick test for OpenGL

This project derives work from http://www.opengl-tutorial.org/ The codes in folder "common" are all copied from the original site.

This project aims to setup a convenience of quick test.

MacOSX you should add the run-script to your generated project manually.
echo "About to do it!"
#echo ${SRCROOT}
cp -R ${SRCROOT}/exec ${TARGET_BUILD_DIR}
cp -R ${SRCROOT}/shaders ${TARGET_BUILD_DIR}
cp -R ${SRCROOT}/objects ${TARGET_BUILD_DIR}
cp -R ${SRCROOT}/../resource ${TARGET_BUILD_DIR}
