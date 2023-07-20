# Description of internals of shuffling

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