GSoC Qualification Task for Implementing Symplectic Integrators in Geant4
====

## Building the Project

### Pre-Requisites

1. A C++ compiler for C++14 or higher.
2. CMake
3. A Python installation with the relevant development headers (like the `python-dev` apt package on Ubuntu)
4. The Matplotlib Python library

### Instructions
1. Clone the repository.
    * You can clone the repo by running `git clone URL`.
2. Run CMake on the repository.
    * Run `cmake -S. -Bbuild`.
    * Run `cmake --build build`.
        - On Linux, you can also append `-- -j<num>` to the end of the above command, to run the build on `<num>` cores. For example,
        ```bash
        cmake --build build -- -j4  # for 4 cores
        ```
	* Keep in mind that compiling might take a minute or two, due to the template stuff in the project.
3. Run the executable.
    * The executable will be in `build/`. The name of the executable is `solver` (Linux) or `solver.exe` (Windows).

Here is the [link](https://docs.google.com/document/d/1uPMF53IFITruSWTe2Kzr87Ux09wrrIV25iQMLL1c9xE/edit?usp=sharing) to my write-up.
