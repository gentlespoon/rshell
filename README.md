# rshell
By [Calvin Bui](https://github.com/cbui005/) and [Angda Song](https://github.com/gentlespoon/)

Rshell is a Linux command shell implemented in C++.

## Usage
Reads in commands one line at a time. Commands will have the form: 
```
cmd = executable [argumentList][connector cmd]
connector = || or && or ;
```
where executable is an executable program in the PATH and argumentList is a list of zero or more arguments seperated by spaces.
Calling 'exit' exits the shell.

Anything after a '#' is a considered a comment and will be ignored.
## Example commands
```
$ ls -a; echo hello && mkdir test || echo world; git status
```

## Installation
```
$ git clone https://github.com/gentlespoon/rshell.git
$ cd rshell
$ make
$ bin/rshell
```

## License
[GNU License](https://github.com/gentlespoon/rshell/blob/exec/LICENSE)
