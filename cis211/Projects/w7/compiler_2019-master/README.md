# Duck Compiler

This project is a compiler (and an interpreter) Mallard, 
a simple programming language.  

## Recycled code

Out of a sense of environmental responsibility, this compiler is constructed largely from post-consumer waste code, primarily from the Calculator project. However, many parts had to be melted down and recast, and almost every part required at least some small modification. 


## New language features

Mallard has control structures like 
*if/then/else/fi* and *while/do/od*, which 
our calculator did not have.  


* while loops are written like this: 

```
while x > 0 do
    fact = fact * x ;
    x = x - 1 ;
od
```

The boolean expressions to control while loops are very limited:  Just a single comparison, which may be <, >, or ==.  

* if/then/else/fi, with an optional else part.  The condition after the `if` is like the condition in the `while` loop.   

Mallard also has a ```read``` expression 
for reading from the keyboard and a 
```print``` statement for printing. 

An interpreter is provided.  You are building
a code generator for a compiler that generates
code for a Duck Machine 2019W. 

## How to proceed

See docs/HOWTO-compile.md
