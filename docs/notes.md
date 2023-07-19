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

There are currently following classes used to get a reordered sequence of passes from a given one: Driver, PropertyStateMachine, PassListGenerator, PassPrinter, FileReader, PassLogParser, PassToReorderParser, ShuffleOptionParser

## File reading, parsing and printing

FileReader class serves as public base for PassLogParser and PassToReorderParser: it only has a method to read all content from given file into a buffer. PassLogParser
has a method parse\_log() to parse file with info about passes and a method parse\_constraints() to add into existing vector of pass\_info custom constraints.
PassToReorderParser has a method parse\_passes\_file() to parse file with names of passes to reorder. Both of those class give access to resulting vectors using begin() and end() methods.

PassPrinter class takes in file name, prefix & suffix to print the passes with, a range of passes and prints them to given file

ShuffleOptionParser parses command line options (like input file, output file, etc.) and gives access to those options via just public fields of itself (For instance it has std::string input\_file which is set to input file path, if it was given)

## PropertyStateMachine
This struct just keeps the state, applied pass list and can correctly apply futher passes. To work, it takes a map from passes ids to property of corresponding pass in it's constructor
It also has static methods to compress two properties / pass properties structs into one.

## Driver
This is a template class, which takes parser of pass info, parser of file with passes to reorder and generetor of new pass list as template parameters, in current implemetation PassLogParser, PassToReorderParser and PassListGenerator are used when instantiating Driver. Using first two, driver prepares the information about passes and vector of passes to be reordered and gives them to PassListGenerator via set\_pass\_vec() and set\_info\_vec() methods. It then invokes it's method to reorder. After reordering is done, it uses PassPrinter to print out resulting sequence.

Via modify\_subpasses function it modifies how the subpasses(for instance, loop2\* are subpasses of loop2 pass) are printed for plugin to recognize them correctly

Driver also has a possibility of breaking down second list into two (lists before "pre" and after) and to reorder them independently. This was done, because "pre" was a problematic pass: some passes that are before it in original gcc pass sequence should stay before it, the same with some of the passes after it, so to alleviate studying of prohibited pass combinations, those two parts of second list were firslty examined indepedently, and only then combined back with corresponding necessary custom constraints.

## PassListGenerator
PassListGenerator gets a range of descriptions (name, original and custom properties(required, provided, destroyed)) of all passes and a range of passes to reorder by names (via constructor or set\_info\_vec and set\_passes\_vec methods).
During examining gcc's source code, it was discovered that some passes must go together in existing sequence (like loop2\_\* passes) or some of them need preparation passes before them (for loop/noloop need fix\_loops pass), so those passes are reordered together correctly

The pass reordering is done the folowing way:

To optimize the reordering process a bit all given passes are firstly mapped onto id's.

The starting state and expected minimal ending state are taken as arguments in shuffle\_pass\_order() method

First a PropertyStateMachine with given state as starting is created. Then a map from unique property requirement to vector of passes (from given list to reorder), which demand this property is made. 
To choose new pass, PropertyStateMachine property fields are observed, from the aforementioned map all passes that can be satisfied with observed state are taken, and one of them is chosen randomly. It is applied using PropertyStateMachine method and it is deleted from the unique property requirement to vector of passes map.
This process goes on, until there are no passes left, or none of left passes are satisfied by current property state. In the second case; the generated sequence is dropped and we try again (we strive to use all passes from given sequence). If after TRY\_AMOUNT(=10000 in current implementation) tries the resulting sequence still does not include all passes, based on fail\_if\_not\_all\_passes\_used flag value, either COULD\_NOT\_GEN constant is returned as an error code, or just the last sequence is left as final. Then, a resulting vector of reordered passes is mapped back from id's to names (and some of id's are mapped to vectors of names). The PassListGenerator has begin() and end() methods to access this vector

PassListGenerator also has a verify() method, which takes in starting property and a range of passes names to verify correct order of. It creates PropertyStateMachine and via it applies passes from file to given state and reports back if error occured.

## The program to shuffle
In main(), firstly an instance of ShuffleOptionParser is created and it gets information about necessary files from command line, or reports an error if there were not enough input options. Then, an instance of Driver is created with PassLogParser, PassToReorderParser and PassListGenerator as it's template parametres. 
After that, whether to breakdown second list flag is set, generate\_file\_with\_shuffle() method of driver is called(which does all of the reordering) and it's return value is returned from main()


# Testing and benchmarks

To test whether compilation with reordered passes was successful, a testing script was written. It compiles with phase reordering, runs the program on a dataset, 
and then compares results with a program built just with -O2 flag. For testing we utilized cBench 1.1 benchmarks bzip2 (both compression and decompression) and gsm. 
Each list with its constraints was run several hundred times without any errors before it was considered stabilized by the constraints. Then all lists together were tested the same way.

A perfomance benchmarking utility was created with bzip2. Passes were reordered randomly (respecting the constraints) and the size and execution time of resulting binary were observed. 
Over several hundred iterations, best results we got were ~1.5% improvements in both size and perfomance.
These values were expected to minor, as random shuffling is unlikely to produce great results, however it shows that it is possible to improve compilation results by utilizing phase reordering in GCC.

# Achieved results 

 - Implemented plugin that allows pass tree manipultaion
 - Implemented shuffler to generate pass orders conforming to compilator requirements 
 - Determined reqired constraints on pass orders
 - Random shuffle testing yielded minor improvements in size and perfomance



