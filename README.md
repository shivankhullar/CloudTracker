# CloudTracker

CloudTracker is a tool designed for tracking collections of particles/cells in fluid simulations. 
It's a tool that can be used in post processing to track collections that have already been identified. 
You would have to first use some other tool to identify collections of particles/cells, save them in an HDF5 file 
with a specific format (see [Documentation](#documentation) for details.)
It works in two steps: 1) It matches (see [Matcher](#matcher))


## Table of Contents
- [Installation](#installation)
- [Usage](#usage)
- [Project Structure](#project-structure)
- [Modules](#modules)
  - [Linker](#linker)
  - [Matcher](#matcher)
- [Documentation](#documentation)
- [Testing](#testing)
- [Contributing](#contributing)

## Installation

To install CloudTracker, just clone the repository:

**Clone the repository**:
    ```sh
    git clone https://github.com/yourusername/CloudTracker.git
    cd CloudTracker
    ```
You would have to provide 

## Usage



**Build the project**:
    ```sh
    make 
    ```

After building the project, you can run the main executables located in the `bin` directory. Usage details for each module are provided below.

## Project Structure
CloudTracker/
├── docs/                    # Documentation files
├── src/                     # Source code files
│   ├── linker/              # Linker related source code
│   │   ├── io/              # I/O handling code for linker
│   │   ├── utils/           # Utility functions for linker
│   │   └── main/            # Main linker functionality
│   ├── matcher/             # Matcher related source code
│   │   ├── io/              # I/O handling code for matcher
│   │   ├── utils/           # Utility functions for matcher
│   │   └── main/            # Main matcher functionality
│   ├── Makefile.systype     # Makefile for system type detection
├── Makefile                 # Makefile for building the project
└── README.md                # Project README file



## Modules
# Linker
asfhousdhgiuadhf
# Matcher
Aasifhoadhfiausfh
