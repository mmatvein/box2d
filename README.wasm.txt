```bash
mkdir build
cd build
emcmake cmake .. -DBOX2D_BUILD_UNIT_TESTS=OFF -DBOX2D_BUILD_DOCS=OFF -DBOX2D_BUILD_TESTBED=OFF
emmake make
../generate_idl_bindings.sh
../build_web.sh
```