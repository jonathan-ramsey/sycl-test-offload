# sycl-test-offload
A sandbox for testing a SYCL-enabled DLL + a non-SYCL test program

### Prerequisites:

- Only tested on Windows 10 Pro
- Either the Intel oneAPI DPC++ compiler or a successful build of the `sycl` branch of the `intel/llvm` github repo.
    - For NVIDIA GPU offloading, the self-built `intel/llvm` compiler must have been configured with `--cuda`.
- CMake
- Ninja build system
- Boost
- CUDA + the NVIDIA GPU Computing Toolkit (for NVIDIA GPU offloading)

### Step 1: Compile the SYCL-enabled DLL

Starting from the root directory of this repo.

#### Option A: Intel oneAPI pre-packaged DPC++ compiler/toolkit

- In a fresh command prompt, invoke the `setvars.bat` script from your oneAPI installation (e.g. "`C:\Program Files (x86)\Intel\oneAPI\setvars.bat`")
- `cd lib`
- `mkdir build && cd build`
- `cmake -G"Ninja Multi-Config" .. -DBOOST_ROOT=c:\dev\code -DCMAKE_INSTALL_PREFIX=c:\dev\sycl-test-offload --fresh`
- `cmake --build . --config Debug --clean-first`
- `ninja install`

**NB: Make sure you update the paths for your own system.**

#### Option B: Custom build of the `sycl` branch of the `intel/llvm` github repo.

- In a fresh command prompt, invoke the `set_env_vars_clang.bat` script in the root directory of this repo (NB: Be sure to update paths to point to where you built the intel/llvm SYCL-enabled compiler!)
- `cmake -G"Ninja Multi-Config" .. -DBOOST_ROOT=c:\dev\code -DCMAKE_INSTALL_PREFIX=c:\dev\sycl-test-offload -DCMAKE_C_COMPILER=clang-cl.exe -DCMAKE_CXX_COMPILER=clang-cl.exe --fresh`
- `cmake --build . --config Debug --clean-first`
- `ninja install`

### Step 2: Compile the non-SYCL test program

Starting from the root directory of this repo.

- If using a fresh terminal, be sure to invoke either `set_env_vars_clang.bat` or `set_env_vars_intel.bat` in the repo root folder
- `mkdir build && cd build`
- If using a self-built `intel/llvm` Clang:
    - `cmake -G"Ninja Multi-Config" .. -DBOOST_ROOT=c:\dev\code -DCMAKE_INSTALL_PREFIX=c:\dev\sycl-test-offload -DCMAKE_C_COMPILER=clang-cl.exe -DCMAKE_CXX_COMPILER=clang-cl.exe --fresh`
- If using oneAPI:
    - `cmake -G"Ninja Multi-Config" .. -DBOOST_ROOT=c:\dev\code -DCMAKE_INSTALL_PREFIX=c:\dev\sycl-test-offload --fresh`
- If using MSVC/CL:
    - `cmake -G "Visual Studio 17 2022" .. -DBOOST_ROOT=c:\dev\code -DCMAKE_INSTALL_PREFIX=c:\dev\sycl-test-offload --fresh`
- `cmake --build . --config Debug --clean-first`
- `ninja install` (if using MSVC, add a `--target INSTALL` to the build command instead)

### Step 3: Running the test program

- Change to the `bin` directory off of the root directory of this repository (required at the moment);
- Invoke `set ONEAPI_DEVICE_SELECTOR=opencl:cpu` (for CPU, and the most robust option for testing)
    - If doing NVIDIA GPU offloading, use `set ONEAPI_DEVICE_SELECTOR=cuda:*`
- Run `sycl_test_offload.exe`

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

- NB: The path and name of the DLL is currently hard-coded in `sycl_test_offload.cpp`! :P It also assumes that the DLL is in the `bin` directory with the executable.
- This test program is inspired by the [Hello SYCL example from ComputeCpp](https://developer.codeplay.com/products/computecpp/ce/2.11.0/guides/sycl-guide/hello-sycl) and the `vector-add-buffers` example from the `oneapi-src/oneAPI-samples` github repository.
