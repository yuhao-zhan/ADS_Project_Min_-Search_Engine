# README

This is the README file for instructions on building the executable file and how to run it.

1. First of all, we'd like to inform you that the environment we use is **Clion**, which includes the basic environment elements to configure like `cmake-build-debug`, `CMakeLists.txt` and so on. 

   * We recommend you to run this whole project directly in Clion, and you can change the `CMakeLists.txt` to determine which files are executable and whether you want to test the correctness (let `test.c` executable while `main.c` not) or just making the search (let `main.c` executable while `test.c` not) .
   * If you don't configure the Clion environment, you can directly copy and build the project in your Vscode or IDE C++, which is also not so hard.

2. Secondly, we want to introduce each file and subdirectory in `code` folder.

   * `header_files`: stores all the header files used in this project.
   * `source_files`: stores all the source **.c** files used in this project.
   * `other_files`: stores all the files needed to achieve complete functions.
     * `GroundTruthDataMaking.sh`: generating ground truth data.
     * `VisualizeResults.ipynb`: visualize the results.

   * `txt`: stores all the documents we have curated by **Web Crawler.**
   * `search_output.json`: our ground truth data for test.
   * `build_new` and `cmake-build-debug`: Come with Clion environment.
   * `CMakeLists.txt`: Comes with Clion environment. You can determine which programs are executable or not to achieve different functions.

   > [!NOTE]
   >
   > Following part in `CMakeLists.txt` is used for reading and parsing the `.json` format files in the project, such as the ground truth data and so on.
   >
   > <<<NOTICE>>> You must install some specific package like `json-c` in your computer so that you can run this and the whole project smoothly. 
   >
   > ```cmake
   > # Manually set the path for json-c library
   > include_directories(/opt/homebrew/Cellar/json-c/0.18/include/json-c)
   > link_directories(/opt/homebrew/Cellar/json-c/0.18/lib)
   > 
   > # Link the json-c library
   > target_link_libraries(MiniSearchEngine /opt/homebrew/Cellar/json-c/0.18/lib/libjson-c.dylib)

   ## Detailed Installation Steps (only for reference!!! If this fails, ask GPT4 or Goodle!)

   ### - **MacOS Installation (Homebrew Method)**

   #### Step 1: Install json-c using Homebrew

   1. **Install Homebrew** (if you haven't already):

      ```bash
      bash
      
      /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
      ```

   2. **Install json-c**:

      ```bash
      bash
      
      brew install json-c
      ```

   3. **Verify the installation**:

      ```bash
      bash
      
      brew info json-c
      ```

      You'll see the installation path, usually something like:

      ```bash
      bash
      
      /opt/homebrew/Cellar/json-c/0.18
      ```

   #### Step 2: Modify `CMakeLists.txt` to Include json-c

   In your `CMakeLists.txt`, manually set the include and link paths to `json-c`:

   ```cmake
   # Manually set the path for json-c library
   include_directories(/opt/homebrew/Cellar/json-c/0.18/include/json-c)
   link_directories(/opt/homebrew/Cellar/json-c/0.18/lib)
   
   # Link the json-c library
   target_link_libraries(MiniSearchEngine /opt/homebrew/Cellar/json-c/0.18/lib/libjson-c.dylib)
   ```

   #### Step 3: Rebuild Your Project

   Navigate to your project directory and run:

   ```shell
   mkdir build
   cd build
   cmake ..
   make
   ```

   This will compile your project using `json-c` in the specified paths.

   ------

   ### - **Windows Installation (Using vcpkg)**

   #### Step 1: Install vcpkg (Package Manager)

   1. **Install `git`** (if not installed) from https://git-scm.com/download/win.

   2. **Clone vcpkg**:

      ```bash
      bash
      
      git clone https://github.com/microsoft/vcpkg.git
      cd vcpkg
      ```

   3. **Bootstrap vcpkg**:

      ```bash
      bash
      
      ./bootstrap-vcpkg.bat
      ```

   4. **Install `json-c`**:

      ```bash
      bash
      
      ./vcpkg install json-c
      ```

   #### Step 2: Modify `CMakeLists.txt` for Windows

   ```cmake
   # Add vcpkg include and link paths for json-c
   include_directories(${CMAKE_SOURCE_DIR}/vcpkg/installed/x64-windows/include)
   link_directories(${CMAKE_SOURCE_DIR}/vcpkg/installed/x64-windows/lib)
   
   # Link json-c library
   target_link_libraries(MiniSearchEngine json-c)
   ```

   In Windows, `vcpkg` manages the library paths, so as long as you run CMake correctly with vcpkg's toolchain, it will automatically pick up the `json-c` libraries.

   #### Step 3: Rebuild the Project with vcpkg

   When using vcpkg, you need to pass the toolchain file to CMake:

   1. Rebuild your project

      ```bash
      bash
      
      mkdir build
      cd build
      cmake -DCMAKE_TOOLCHAIN_FILE=../vcpkg/scripts/buildsystems/vcpkg.cmake ..
      cmake --build .
      ```

   ------

   ### **Common Troubleshooting Steps**

   1. **Ensure Correct Paths**:

      - Always double-check that the paths in `CMakeLists.txt` point to the correct directories (e.g., `/opt/homebrew/Cellar` for MacOS or vcpkg paths for Windows).

   2. **Check Environment Variables**:

      - If paths are not being picked up automatically, export environment variables manually:

        - MacOS:

          ```bash
          bash
          
          export CPATH=/opt/homebrew/Cellar/json-c/0.18/include/json-c
          export LIBRARY_PATH=/opt/homebrew/Cellar/json-c/0.18/lib
          ```

        - Windows: Set the include and library paths in CMake manually or via system settings if needed.

   3. Remember to **include** the `json-c` in needed source file.

      ```c
      #include "/opt/homebrew/Cellar/json-c/0.18/include/json-c/json.h"
      ```

      

   4. **Linker Issues**:

      - If you encounter linker errors like `library not found`, ensure that the paths set in `CMakeLists.txt` match the locations of the `.dylib` (MacOS) or `.lib` (Windows) files.

   

3. Thirdly, about the **file path problem**. You should open the `code` directory as a project and run it so that the relative path can be located correctly, like `../txt` and so on. If you find it reports you Error of "cannot find such file or directory", don't panic, just adjust the path slightly so that it can fit to the file or directory in your own computer, which is not so hard.
