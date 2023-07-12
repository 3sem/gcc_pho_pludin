# GCC phase reorder plugin

## Description

Plugin currently implements dumping, clearing and altering gcc pass tree. Functionality was tested on gcc 7.3.0 built for aarch64 target.

## Usage

Compiled plugin.so file should be provided to the gcc using -fplugin= argument (it does not support ~ expansion). 
Plugin arguments are provided with -fplugin-arg-plugin-\<name\>=\<arg\>. For list of possible arguments see help (-fplugin-arg-plugin-help), it will be printed to stderr. 
Providing a compilable file is required for plugin to work. 

## Capabilites

### Pass dumping

Several formats of dumps are supported, such as human- or machine- readable dump of all passes, or dump of only executed passes.

### Pass tree clearing

Clearing full pass tree is possible with a separate argument, but not recommended, as in results in internal compiler errors.
These error are unavoidable as gcc does not differentiate reqired and optional passes and does not protect those passes, which are required for compilation.

More precise control of pass removal is possible with help of marker passes. These passes are inserted by plugin and gate status of passes between them is overriden to 0, causing these passes not to run.
It is the only found viable way of removing (disabling) passes, as internal compiler structure does not support actual deletion from pass tree, 
and true deletion cannot be implemented without patching compiler source code.

### Pass insertion

Plugin allow insertion of passes in form of 3 pass lists. When using pass insertion 3 premarked lists of optimization passes are removed from pass tree 
and then can be replaced with passes from corresponding pass list. These list files should be named list1.txt, list2.txt, list3.txt and should contain names of passes in order of insertion.
There are several constraints on these lists: passes should not be moved from one list to another, and there are some passes that should be moved together or always be present in a list.
Plugin supports one level of nesting. Lines starting with '>' are inserted as a subpass line to the latest pass without '>'.

## Pass insertions details

### List places

After pass tree analysis (passes.def in gcc sources) and some experiments, three major lists of optimizational passes have been separated. These lists correspond to inter-procedural optimizations,
per-function optimizations and late RTL optimizations. Exact places of marker passes can be seen by utilizing pass dumping with active -fplugin-arg-plugin-pass\_replace option.


Current marker positions:
	- before inline\_param #1
	- after inline\_param #2
	- after \*strip\_predict\_hints
	- after local-pure-const 
	- after dfinit
	- before init-regs

### Constraints

Pass reordering end removal within these lists is mostly free with some constraints, such as:
	- loop subpasses should start with loop\_init and end with loop\_done
	- loop should be immediately preceded with fix\_loop
	- inline\_param should be immediately preceded with \*rebuild\_cgraph\_edges
	- loop2 ans its subpasses loop2\_init, loop2\_invariant, loop2\_done should always be in the third list 
	- loop2 subpasses should start with loop2\_init and end with loop2\_done
