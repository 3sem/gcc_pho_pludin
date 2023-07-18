# Description of internals of shuffling

## Problem statement
IR in gcc is partially described with "properties". This is a bit mask, that shows in which state IR currently is (is it in ssa form, or was it already lowered 
to rtl, etc.). Internal passes structures have, among other things, fields that tell which properties of IR are required for this pass, which one it creates 
during it's work and which ones it destroyes. Obviously, because of this pass reordering in gcc cannot be done with just random permutations of passes: the  pass sequence generator needs to track property state after each taken pass, and choose futher pass taking into consideration it's required properties

## Class hierarchy

### Concise description
There are currently following classes: Driver, PropertyStateMachine, PassListGenerator, PassPrinter, FileParser, PassLogParser, PassToReorderParser

The PropertyStateMachine responsibilty is just to store already current reordered sequence of passes, property mask after applyng those passes and to correctly apply new given pass

PassListGenerator reorders given pass list, using PropertyStateMachine to apply passes and chooses each new pass in new sequence so that it's required properties would be satisfied

Driver is just a small class over PassListGenerator, which gives correct files with passes list and prints out passes lists to correct files

PassPrinter prints pass list to given file with small adjustments needed for plugin to worK

FileParser is a base class for PassLogParser and PassToReorderParser which parse files with info about passes and with passes to reorder respectively


### PassListGenerator
PassListGenerator gets a range of descriptions (name, original and custom properties(required, provided, destroyed)) of all passes and a range of passes to reorder by names (via constructor or set_info_vec and set_passes_vec methods).
Due to gcc's quirks, some passes must go together in existing sequence (like loop2_* passes) or some of them need preparation passes before them (for instance inline_param needs rebuild_cgraph_edges), so those passes are given a single same id, so the whole group would be treated as a single pass in reordering

#### Pass reordering itself
The pass reordering from a given starting property state is done the folowing way:

To optimize the reordering process a bit all given passes are firstly mapped onto id's.

The starting state and expected minimal ending state are taken as arguments in corresponding method to shuffle

First a PropertyStateMachine with given state as starting is created. Then a map from unique property requirement to vector of passes (from given list to reorder), which demand this property is made. 
To choose new pass, PropertyStateMachine property fields are observed, from the aforementioned map are taken all passes that can be satisfied with observed state, and one of them is chosen randomly. It is applied using PropertyStateMachine method and it is deleted from unique property requirement to vector of passes map.
This process goes on, until there are no passes left, or none of left passes are satisfied by current property state. In the second case; the generated sequence is dropped and we try again (we strive to use all passes from given sequence). If after TRY_AMOUNT(=100) tries the resulting sequence still does not include all passes, based on fail_if_not_all_passes_used flag value, either COULD_NOT_GEN constant is returned as an error code, or just the last sequence is left. Then, a resulting vector of reordered passes is mapped back from id's to names (and some of id's are mapped to vectors of names). The PassListGenerator has begin() and end() methods to access this vector

PassListGenerator has a verify() method, which takes in starting property and a range of passes names to verify correct order of. It creates PropertyStateMachine and via it applies passes from file to given state. PropertyStateMachine will print to std::cerr if a mistake is found


### PropertyStateMachine
This struct just keeps the state, applied pass list and can correctly applies futher passes. To work, it takes a map from passes ids to property of corresponding pass in it's constructor
It also has static methods to compress two properties / pass properties structs into one.

### PassPrinter
This class takes in file where to dump, and prefix/suffix to print passes with. Also, it takes into account some formating needed for plugin to work correctly (for example: loop2_* are all subpasses of loop2, and the plugin requires subpasses to be printed with '>' to be parsed correctly)

### Driver
This is a template class, which takes log parser and file with passes to reorder parser, via them prepares the information about passes and vector of passes to be reordered for PassListGenerator. It correctly sets the starting state for PassListGenerator and invokes it's method to reorder. It uses PassPrinter to print out resulting sequences.

It handles some corner cases, which were dicovered when gcc's source code was examined: for example, "loop" optimization has a big sub-list of optimizations which can also be reordered, but this sub-list must be printed with '>' and must all be after "loop" optimization for plugin to work correctly; so if Driver in resulting sequence encounteres "loop" optimization, it prints that whole sub-list after it, and can reorder it beforehand (has a flag to show whether we want to reorder that sub-list or not)

### PassLogParser 
This class has a method parse_log() to parse file with info about passes and a method parse_constraints() to add into existing vector of pass_info custom constraints. This class gives access to its pass_info vector through begin() and end() methods

### PassToReorderParser
This class has a method parse_passes_file to parse file with names of passes to reorder, and gives access to range of those passes via begin() and end() methods
