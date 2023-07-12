# Description of internals of shuffling

## Problem statement
IR in gcc is partially described with "properties". This is a bit mask, that shows in which state IR currently is (is it in ssa form, or was it already lowered 
to rtl, etc.). Internal passes structures have, among other things, fields that tell which properties of IR are required for this pass, which one it creates 
during it's work and which ones it destroyes. Obviously, because of this pass reordering in gcc cannot be done with just random permutations of passes: the  pass sequence generator needs to track property state after each taken pass, and choose futher pass taking into consideration it's required properties

## Class hierarchy

### Concise description
There are currently 4 classes: Driver, PropertyState, PassListGenerator and PassDumper

The PropertyState responsibilty is just to store already current reordered sequence of passes, property mask after applyng those passes and to correctly apply new given pass

PassListGenerator reorders given pass list, using PropertyState to apply passes and chooses each new pass in new sequence so that it's required properties would be satisfied

Driver is just a small class over PassListGenerator, which gives correct files with passes list and prints out passes lists to correct files

PassDumper prints pass list to given file with small adjustments needed for plugin to worK


### PassListGenerator
PassListGenerator gets a range of descriptions (name and required, provided, destroyed properties) of all passes and a range of passes to reorder by names. To optimize the reordering process a bit all given passes are firstly mapped onto id's.
Due to gcc's quirks, some passes must go together in existing sequence (like loop2_* passes) or some of them need preparation passes before them (for instance inline_param needs rebuild_cgraph_edges), so those passes are given a single same id, so the whole group would be treated as a single pass in reordering

#### Pass reordering itself
The pass reordering from a given starting property state is done the folowing way:

First a PropertyState with given state as starting is created. Then a map from unique property requirement to vector of passes (from given list to reorder), which demand this property is made.

To choose new pass, PropertyState property field is observed, from the aforementioned map are taken all passes that can be satisfied with observed state, and one of them is chosen randomly. It is applied using PropertyState method and it is deleted from the map of unique property requirement to vector of passes.
This process goes on, until no passes are satisfied by current property state.

Then, a resulting vector of reordered passes is mapped back from id's to names (and some of id's are mapped to vectors of names). The PassListGenerator has begin() and end() methods to access this vector

PassListGenerator has a verify() method, which takes in starting property and a file name with pass list to verify correct order of. It creates PropertyState and via it applies passes from file to given state. PropertyState will print to std::cerr if a mistake is found


### PropertyState
This struct just keeps the state, applied pass list and can correctly applies futher passes. To work, it takes a map from passes ids to property of corresponding pass in it's constructor

### PassDumper
This class takes in file where to dump, and prefix/suffix to print passes with (developer was told this could be needed). Also, it takes into account some formating needed for plugin to work correctly (for example: loop2_* are all subpasses of loop2, and the plugin requires subpasses to be printed with '>' to be parsed correctly)

### Driver
This class prepares the information about passes and vector of passes to be reordered for PassListGenerator. It correctly sets the starting state for PassListGenerator and invokes it's method to reorder. It uses PassDumper to print out resulting sequences.

It handles some corner cases, which were dicovered when gcc's source code was examined: for example, "loop" optimization has a big sub-list of optimizations which can also be reordered, but this sub-list must be printed with '>' and must all be after "loop" optimization for plugin to work correctly; so if Driver in resulting sequence encounteres "loop" optimization, it prints that whole sub-list after it, and can reorder it beforehand (has a flag to show whether we want to reorder that sub-list or not)