# CloudTracker

CloudTracker is a tool designed for tracking collections of particles/cells (called clouds for the purposes of this code) in fluid simulations. Originally designed for galaxy formation simulations, but can be applied to other contexts easily.
It's a tool that can be used in post processing to track collections that have already been identified. 
You would have to first use some other tool to identify collections of particles/cells in each snapshot, save them in an HDF5 file with a specific format (see [Documentation](#documentation) for details.)

It works in two steps: 
1) It matches (see [Matcher](#matcher)) clouds in two snapshots and identifies parent-child edges (if you think of it as a graph). It currently does so for consecutive snapshots, but the code can be modified easily to have arbitrary spacing between snapshots.
2) It links (see [Linker](#linker)) clouds identified as a parent-child pair (a parent can have more than one child, see below for details on the algorithm). The code follows the algorithm listed below, but would have to be modified if you want a different method. One can write python code to work with matcher outputs and that may be easier if you want a different way of linking clouds together in chains.  

The code has been divided into these two parts so that it is easier to maintain more control over each part of the process. It is useful if someone wants to use just a part of the code (like the matcher), but wants to define their own way of linking entities together.

## Table of Contents
- [Installation](#installation)
- [Project Structure](#project-structure)
- [Modules](#modules)
  - [Linker](#linker)
  - [Matcher](#matcher)
- [Documentation](#documentation)
- [Usage](#usage)


## Installation
To install CloudTracker, just clone the repository:
```
git clone https://github.com/yourusername/CloudTracker.git
cd CloudTracker
```
You will have to configure the Makefile and add the paths to the required libraries. Use Makefile.systype and Makefile to store settings for your machine. If you're running this on a Mac, the default paths provided should work if you install hdf5 by doing
```
brew install hdf5
```


## Project Structure
```
|CloudTracker/
|---|include/             # Contains header files
|---|docs/                # Documentation files
|---|src/                 # Source code files
|   |---|linker/          # Linker related source code
|   |   |---|io/          # I/O handling code for linker
|   |   |---|utils/       # Utility functions for linker
|   |   |---|main/        # Main linker functionality
|   |   |---|Makefile     # Makefile for building the linker program
|   |---|matcher/         # Matcher related source code
|   |   |---|io/          # I/O handling code for matcher
|   |   |---|utils/       # Utility functions for matcher
|   |   |---|main/        # Main matcher functionality
|   |   |---|Makefile     # Makefile for building the matcher program
|   |---|Makefile.systype # Makefile for system type detection
|---|README.md            # Project README file
|---|Documentation.mk     # Makefile to generate reference manual
```

## Modules
### Matcher
This part of the code will match clouds in subsequent snapshots by checking for the particles of each pair of clouds. 
This is an $\mathcal{O}(n^2)$ calculation where $n$ denotes the number of clouds in a snapshot. 
Fortunately, it is still very fast (thanks to being written in C++). This process is easily 
parallelizable, but currently serial. Parallel support with OpenMP will be added in the future. 
The matcher program will create HDF5 files which contain information about the children and the 
amount of "mass" they derive from each of their parents (a parent is any cloud that donates
at least one particle to the child cloud). It also contains information about the parents and the
amount of "mass" they give to each of their child cloud. 
If you want CloudTracker to just match based on number and not masses, 
set up your masses array in the input file such that it is the same value for each cell/particle.

### Linker
This part of the code will link together the clouds that matcher matches. The exact algorithm is described in a 
companion paper, but briefly it proceeds as follows:
- Start from the first snapshot, snapshot A and load all clouds to THE LIST (think of it as a list of lists) in descending order by mass.
- Proceed down the list, for each cloud X identify a "proper child" in snapshot B and add it to the list of descendants of cloud X, which is a part of THE LIST.
  - If cloud X has more than one child, we choose a child Y with the most mass donated from the parent.
  - If child cloud Y is a child of a more massive parent and already exists in THE LIST, the next most massive child of cloud X is chosen.
- When a "proper child" of a cloud is not found, the bloodline ends and no further descendants of that cloud exist.
- If there are clouds in snapshot B that are not a descendant of any cloud X in snapshot A, then add them to THE LIST. 

In this method of linking clouds, if two clouds undergo a merger, we consider the less massive cloud to be dead.


## Documentation

CloudTracker comes with Doxygen support. You can find a reference manual in the *docs* directory. 
If you want to create that from scratch, a Documentation.mk file is provided. You can generate the manual by doing
```
make -f Documentation.mk
```



## Usage

CloudTracker is designed to work with a certain convention for the input files. Here's the HDF5 structure for input files:

```
Filename: 
"JKL_yyy_n10_alpha2.hdf5"
|GROUP "/" 
|---|GROUP "CloudXYZ" 
|---|---|GROUP "ParticleSubgroup" 
|---|---|---|DATASET "Masses" 
|---|---|---|DATASET "ParticleIDs"
```
where ```yyy``` is a number that corresponds to the snapshot. 

You also need to have an auxiliary file (it can be a simple .txt file) which contains information about your collections of particles/cells. 
It is used in counting the total number of clouds quickly.
The code will count each line that doesn't start with ```#```. It doesn't matter what is contained in that file, as long as the number of lines 
match the total number of clouds in the snapshot.
You can either modify the ```find_num_clouds()``` function to calculate the total number of clouds, or just make a file like I described above. 

Directory structure needed:
```
|ABC
|---|PQR 
|---|---|UVW
|---|---|---|CloudPhinderData
|---|---|---|---|DEF
|---|---|---|---|---|JKL_yyy_DEF.hdf5
|---|---|---|---|---|MNO_yyy_DEF.dat
```


Here's an example of the matcher parameter file matcher_params.txt
```
path= ./ABC/PQR/                                      # Path to where the simulation UVW is
first_snap = 100                                      # Modify as needed
last_snap = 200                                       # Modify as needed
cloud_prefix = Cloud                                  # This is the prefix for the names of the clouds
dat_filename_base_prefix = MNO_                       # This is the prefix for the .dat auxiliary file
dat_filename_base_suffix = .dat                       # This is the suffix for the .dat auxiliary file 
filename_base_prefix = JKL_                           # Prefix for the .hdf5 file
filename_base_suffix = .hdf5                          # Suffix for the .hdf5 file
file_arch_root = /                                    # Structure of the hdf5 file (see above for the hdf5 file structure)
file_arch_cloud_subgroup = ParticleSubgroup           # Name of subgroup which contains the datasets
file_arch_masses_field = Masses                       # Name of the dataset that contains the masses of the clouds
file_arch_pIDs_field = ParticleIDs                    # Name of the dataset that contains the particleIDs of the clouds
file_arch_pIDgen_field = ParticleIDGenerationNumber   # Field not used currently
write_filename_base_prefix = Tracked_Clouds_          # Output filename prefix for the file produced by matcher
write_filename_base_suffix = .hdf5                    # Output filename suffix for the file produced by matcher
particle_lower_limit = 32                             # To exclude clouds with less than a certain number of cells/particles 
```

The linker parameter file is exactly identical, except for these additions
```
threshold_frac_for_child = 0.3                        # Decides what fraction of mass should be contained in a child for it to be considered at all
linker_output_filename_prefix = Linked_Clouds_        # Prefix for the output file. Output is stored in the same directory that the data is in.
```

Matcher will then create hdf5 files and save it in the directory the cloud data is in, ```DEF``` in our example above. 

Once you have everything configured, you can run matcher or linker by doing 
```
./matcher <config_filename> <name> <sim_name>
```
or 
```
./linker <config_filename> <name> <sim_name>
```
where ```<name>``` is ```DEF``` and ```<sim_name>``` is ```UVW```. ```<config_filename>``` is the .txt parameter file defined above. 