# sycl-test-offload
A sandbox for testing a SYCL-enabled DLL + a non-SYCL test program

### Prerequisites:

- Either the Intel oneAPI DPC++ compiler or a successful build of the `sycl` branch of the `intel/llvm` github repo.
- CMake
- Ninja build system
- Boost

### Step 1: Compile the SYCL-enabled DLL

Starting from the root directory of this repo.

#### Option A: Intel oneAPI pre-packaged DPC++ compiler/toolkit

- In a fresh command prompt, invoke the `setvars.bat` script from your oneAPI installation (e.g. "`C:\Program Files (x86)\Intel\oneAPI\setvars.bat`")
- `cd lib`
- `mkdir build && cd build`
- `cmake -GNinja .. -DBOOST_ROOT=c:\dev\boost -DCMAKE_INSTALL_PREFIX=c:\dev\sycl-test-offload --fresh`
- `cmake --build . --config Debug --clean-first`
- `ninja install`

**Make sure you update the paths for your own system.**

#### Option B: Custom build of the `sycl` branch of the `intel/llvm` github repo.

- In a fresh command prompt, invoke the `set_env_vars_clang.bat` script in the root directory of this repo (NB: Be sure to update paths for your own system!)
- 

### Step 2: Compile the non-SYCL test program

Starting from the root directory of this repo.

- `mkdir build && cd build`
- `cmake -GNinja .. -DBOOST_ROOT=c:\dev\code -DCMAKE_INSTALL_PREFIX=c:\dev\sycl-test-offload --fresh`
- `cmake --build . --config Debug --clean-first`
- `ninja install`

### Step 3: Running the test program

- Change to the `bin` directory off of the root directory of this repository;
- Run `sycl-test-offload.exe`

Expected output:

    Attempting to load plugin: sycl_test_lib_d.dll
    Loaded plugin: SyclTestPlugin
    Offloading time!
    Running on Intel(R) Core(TM) i7-10875H CPU @ 2.30GHz
    Entering queue...
    Creating accessors.
    Starting computation.
    Finished computation.
    Exited queue.
      A { 1, 2, 3, 4 }
    + B { 4, 3, 2, 1 }
    ------------------
    = C { 5, 5, 5, 5 }

NB: Naturally, the device name will change depending on your own system.

### Additional notes:

- Inspired by the [Hello SYCL example from ComputeCpp](https://developer.codeplay.com/products/computecpp/ce/2.11.0/guides/sycl-guide/hello-sycl).
