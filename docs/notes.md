# Problem 

GNU GCC, unlike clang, does not directly support compilation phase reordering. The only way to impact which passes are being run is using command line arguments, 
but these arguments do not provide any control of the order in which passes are run.
However, some part of pass manager are exposed through plugin API, and the task was to research the possibility of GCC phase reordering without resortin to rebuilding GCC from source.

# Solution

Developed solution consists of the plugin itself, that interacts with the plugin manager and gives some control over pass tree, and of shuffler program, 
that generates correct pass sequences based on hand-picked constraints. 

# Pass tree structure

In GCC pass order is hardcoded into passes.def file, that is expanded into private pass manager fields when GCC is built.
Furthemore, pointers to pass objects are stored in several other structures of pass manager, some of which only support insertion and not deletion. 
This allows not much flexibility, and passes cannot be removed from the tree. Rebuilding the pass manager or context which contains it does not seem possible from plugin standpoint.
However we can emulate pass deletion by using a plugin callback to alter gate status of the pass, thus disallowing it to run. This is a way of "removing" passes from the tree that is utilized in the plugin.

To insert a pass plugin must use a pseudo-callback and pass the opt\_pass object into it. Pass objects are created using make\_pass\_\<pass\_class\>() functions. 
However, for ease of use, plugin config files contain pass names and not their class names, so a function that creates a corresponding pass from its name is auto-generated based on pass\_makers.conf.
This .conf file contains \<class\>-\<name\> pairs of used passes and was created by parsing GCC sources.

# Pass structure and constraints

As phase reordering is not directly implemented in GCC, it does not differentiate essential and service passes from optimization and removable passes. 
This forces us to carefully pick which passes can and cannot be moved or replaced. 
Most of the internals are made with the idea that pass order is predetermined and unchangable and
the only mechanism of controlling pass requirements are properties - some inbuilt binary masks that are created/destroyed and are checked before pass execution. 
Unfortunately, these constraints do not represent all pass relations and custom constraints had to be implemented in shuffler.

# Pass lists

To avoid breaking the compiler with our pass reorders, we have separated from the pass tree three pass lists, 
which contain mostly independed passes and reorders within them should not affect other essenctial phases of the compilation process. 
Two of these lists contain optimizations that operate on GIMPLE representation and the third conatains passes that work with RTL.

# Plugin

Plugin implements basic functionality of dumping information, inserting and disabling passes.
Main function of the plugin is plugin\_init(), which is called by GCC on plugin statup. 
This function parses command line arguments forwarded to plugin and registers reqiured callbacks. 
Overall only three types of callbacks are used: OVERRIDE\_GATE to disable unwanted passes and get their information,
PASS\_EXECUTION to dump which passes are actually being run, and PASS\_MANAGER\_SETUP to register new passes.

The main plugin mode - pass replacement, operates as following: first, special marker passes are inserted at the start and end of each pass list. 
Then a OVERRIDE\_GATE callback is registered with a function that for every pass between the markers forces gate status to 0 and prevents the pass from running.
After this pass "removal" new lists are read from files and inserted in place of cut lists. They are actually before the list start marker and are not disabled. 

Config files for plugin support one level of nesting: it is possible to chain passes as a sublist to the pass before them, 
and this chain will be able to execute only if its starting pass has been executed.

Plugin does not chech pass lists for correctness, and relies on shuffler to provide pass lists that will not break the compiler.

# Extra constraints

Default GCC pass properties proved to be not enough for shuffler to generate stable pass lists, and so custom properties were implemented. 
Constraints config file syntax allows to set required, provided, and destroyed properties for each pass, as well as set custom properties 
that exist in the beginning of the list and those that should be present after. This approach allowed enough flexibility to add rules that stabilize reorders without reducing pass combination space too much.

All of the constraints where determined by generating pass lists and using them to build one of the benchmarks (cBench bzip2d and gsm), and then testing benchmark operation.
If some combination of passes caused compilation to fail or produce a corrupted executable, lists were studied to find this incorrect combination and add constraints to prevent it from generating.
Finding the pass that causes problems mostly came down to removing passes from the list or moving them around, until the problem was gone. 
This naive approach was chosen because studying internals of each pass to understand how they might affect each other was too time-consuming and unnecessary, as the simple approach yielded acceptable results.

# Shuffler



# Testing and benchmarks
