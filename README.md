# ffscript
Introduce C Lambda(Cλ) scripting language, a scripting language makes C++ program even more flexible.
![C Lambda Icon](https://github.com/VincentPT/ffscript/blob/master/doc/images/C%20Lambda.png?raw=true)

# What ffscript project does?
The ffscript project builds a compiler and a framework that allow users compile and run the Cλ code inside a C++ host program.
Current the project distributes as header files and two bellow static libraries:
* ffscript.lib - contains script compiler, script running engine and framework to embed the script language into a C++ program.
* ffscriptLibrary.lib - contains some built-in custom libraries such as Math library, Geometry library.

# What Cλ language is?
C Lambda language or Cλ in short is a scripting language but it is not like other programming languages designed to work independently. Working independently means the programming language itself can work with all or many specific domains such as IO operations, networking, databases, graphics.

The Cλ language does not work like that. It is depent on what your application offers. The Cλ only offers some very basic operations(defined in ffscript.lib) such as Addition, Subtraction, Multiplication, Division and some others operators like other programming languages do. It also has framework to allow user map functions, types in C++ side to script side.

So, basically, this language is not designed to compete with other languages but supports for C/C++ language. Giving C/C++ programr library  more power, more flexible.

# Why Cλ language?
* C Lambda language is a C-style language, however, it also offers some advance features of C++ language such as constructor, destructor and especialy is Lambda function. So, that's why I choose C Lambda to named it.
* The Cλ language framework and running engine use the same memory layout, calling convention as C and C++. So, there is no cost of marshaling data between its engine and C/C++ program and able to reuse C/C++ functions without wrapper them.
* Since Cλ language has C-style and base on C and C++, it is easy and quickly to learn Cλ language for C,C++ programmers.
* Cλ language library only offers core operations and work only with application specific domain. So, it is very light and suitable to use in a program which don't want its users to do something that is out of the program bound like a game, an education application or an platform in a programming competition.

# Introduce Cλ language.
Any Cλ program can be divided into two bellow parts:
1. C++ part.  
 This part has responsibility for
 * Importing Cλ static library.
 * Registering domain specific types, functions.
 * Compile the script.
 * Running the complied script.

2. Script part.  
 Write the script using supported operations from the library and the application specific domain.

Let's check some main features offered by the language(or the library).

## Operators  
  Support almost of C++ operators.
  <img src="https://github.com/VincentPT/ffscript/blob/master/doc/images/OperatorPrecedences.png?raw=true" width="100%">
  Cλ operator precedence(based on https://en.cppreference.com/w/cpp/language/operator_precedence).
### Register a operator for a exist type.
```
// C++ part
// function register helper object
FunctionRegisterHelper fb(scriptCompiler);
// register operator<< of ostream and std::string
registerOperator<ostream&, ostream&, const string&>(fb, operator<<, "<<", "ostream", "ostream,string&");
```
### Using in the script.
```
// C Lambda part
// cout is an instance of ostream, which is already defined.
cout << "this is a sample string;
```
## Functions.
A function written inside a script can be an mapped function from C++ or a script function itself.
### Mapping a C++ function.
For function println is defined in C++ as bellow:
```
// C++ part
void println(const std::string& rs) {
 // do something
}
```
We can map the function to use it inside the script as bellow:
```
// C++ part
registerFunction<void, const std::string&>(fb, println, "println", "void","string&");
```
And use it inside the script as bellow:
```
// C lambda part
void doSomething() {
    println("Hello world!");
}
void main() {
    doSomething();
}
```

# Licensing.
The project is distributed under MIT license.
For more details, check LICENSE file.
