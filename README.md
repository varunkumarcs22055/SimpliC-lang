# SimpliC - Programming Made Simple!

![Language](https://img.shields.io/badge/language-C%2B%2B11-blue)
![License](https://img.shields.io/badge/license-MIT-green)
![Platform](https://img.shields.io/badge/platform-Windows-lightgrey)

Hey there! Welcome to **SimpliC** - a programming language that actually makes sense!

Tired of confusing syntax? Want to learn data structures without getting lost in complicated code? SimpliC is here to help! We use plain English words instead of cryptic symbols, so you can focus on learning how things work, not wrestling with syntax.

## What Makes SimpliC Cool?

- **Ready-to-Use Data Structures** - Stacks, queues, trees, graphs... they're all built in!
- **Speaks Your Language** - Use words like `make`, `show`, `when` instead of weird symbols
- **Made for Learning** - Perfect for students and anyone learning algorithms
- **Super Easy** - No complicated setup, just write and run!
- **Try Things Out** - Interactive mode lets you test ideas instantly
- **Real Programming Features** - Functions, recursion, everything you need!

## Let's Get Started!

### Running Your First Program

It's super easy! Just open your terminal and type:

```bash
# Run any SimpliC program
simplic.exe myprogram.simplic

# Try our example file
simplic.exe example.simplic

# Want to experiment? Start interactive mode
simplic.exe
```

### Your Very First SimpliC Program

Let's write the classic "Hello World" - but better!

```simplic
start program HelloWorld

make word message = "Hello, SimpliC!"
show message

end program
```

**What you'll see:**
```
Hello, SimpliC!
```

See? No semicolons, no curly braces, just plain English!

## Cool Things You Can Do

### Math & Variables (The Basics)

Math in SimpliC actually makes sense! No more wondering what `*` or `/` means:

```simplic
start program Calculator

make number a = 10
make number b = 20
make number sum = a plus b          # Yes, you literally write "plus"!
make number product = a times b     # And "times" for multiplication!

show "Sum:", sum
show "Product:", product

end program
```

### Making Decisions (If-Else)

Want your program to make choices? Use `when` and `otherwise`:

```simplic
start program AgeCheck

make number age = 25

when age morethan 18
    show "You are an adult"
otherwise
    show "You are a minor"
done

end program
```

See how readable that is? `when age morethan 18` - just like talking to a friend!

### Data Structures (The Fun Stuff!)

This is where SimpliC really shines! All the data structures you need for learning algorithms are built right in.

#### Stack - Last In, First Out

Think of it like a stack of plates. Last one you put on top is the first one you take off:

```simplic
make stack myStack
put 10 into myStack        # Stack: [10]
put 20 into myStack        # Stack: [10, 20]
put 30 into myStack        # Stack: [10, 20, 30]
show myStack.top           # Shows: 30 (the top plate!)
takeout myStack            # Remove the top
show myStack.top           # Shows: 20
```

#### Lists - Arrays Made Easy

Lists hold multiple items and you can sort them super easily:

```simplic
make list numbers = [5, 2, 8, 1, 7]
show numbers               # Shows: [5, 2, 8, 1, 7]
numbers.sort()             # Magic sorting!
show numbers               # Shows: [1, 2, 5, 7, 8]
```

#### Binary Search Tree - Organized Numbers

BSTs keep your numbers organized automatically:

```simplic
make bst tree
tree.insert(50)
tree.insert(30)
tree.insert(70)
tree.insert(20)
show tree.inorder()        # Shows them in order: [20, 30, 50, 70]
```

### Functions & Recursion (Getting Fancy!)

Want to write reusable code? Functions to the rescue! Here's how to calculate factorial:

```simplic
start program Factorial

define factorial with n
    when n lessthan 2
        giveback 1
    done
    giveback n times factorial(n minus 1)
done

make number result = factorial(5)
show "Factorial of 5 is:", result

end program
```

**You'll see:**
```
Factorial of 5 is: 120
```

The cool part? This function calls itself (that's recursion!) and SimpliC handles it perfectly.

## Quick Language Guide

### What Types of Data Can You Use?

SimpliC keeps it simple with these types:

- `number` - Any number (whole or decimal) like `42` or `3.14`
- `word` - Text in quotes like `"Hello"`
- `yesno` - True or false (use `yes` or `no`)
- `list` - A collection like `[1, 2, 3, 4, 5]`
- `stack` - Like a stack of books (last in, first out)
- `queue` - Like a line at a store (first in, first out)
- `set` - A collection with no duplicates
- `map` - Store key-value pairs
- `bst` - Binary Search Tree for organized data
- `linkedlist` - Nodes connected in a chain
- `graph` - Connect different points together

### Math Operations (Just Use Words!)

No more memorizing symbols! Just write what you mean:

- **Math**: `plus`, `minus`, `times`, `divide`, `mod`
- **Comparing**: `morethan`, `lessthan`, `equals`, `notequal`

Examples:
- `5 plus 3` → `8`
- `10 times 2` → `20`
- `age morethan 18` → checks if age is greater than 18

### Important Words (Keywords)

These special words help you write programs:

- `start program` / `end program` - Every program needs these at the beginning and end
- `make` - Create a new variable (like `make number age = 25`)
- `show` - Display something on screen
- `when` / `otherwise` / `done` - Make decisions (if-else)
- `for` / `in` / `done` - Loop through a list
- `define` / `with` / `giveback` - Create your own functions
- `put` / `into` - Add something to a stack or queue
- `takeout` - Remove something from a stack or queue

### What Can Data Structures Do?

Each data structure has special abilities (methods):

| What You Have | What You Can Do | What It Does |
|---------------|-----------------|--------------|
| List | `.sort()` | Sorts your numbers from small to big |
| Stack | `.top` | See what's on top (without removing it) |
| Queue | `.front` | See who's first in line |
| Set | `.add(value)` | Add a new item (no duplicates) |
| BST | `.insert(value)` | Add a number to the tree |
| BST | `.inorder()` | Get all numbers in order |
| LinkedList | `.insert(value)` | Add a new node |
| Graph | `.addNode(id)` | Add a new point |
| Graph | `.addEdge(from, to)` | Connect two points |
| Graph | `.dfs(start)` | Explore the graph (depth-first) |

## Want to Build It Yourself?

### What You Need

- A C++ compiler (we use GCC/MinGW)
- That's it! Nothing else required

### How to Compile

Just run this command in your terminal:

```bash
g++ -std=c++11 -o simplic.exe main.cpp interpreter.cpp dsa.cpp value_impl.cpp
```

### What's Inside the Project?

Here's what each file does:

```
SimpliC/
├── main.cpp              # The starting point & interactive mode
├── interpreter.cpp       # The brain (reads and runs your code)
├── dsa.cpp              # All the data structures
├── value_impl.cpp       # How values work internally
├── simplic.hpp          # Important declarations
├── dsa.hpp              # Data structure declarations
├── example.simplic      # Example program to try
└── README.md            # You're reading it!
```

## Sharing SimpliC with Friends

Want to share SimpliC without making people compile it? Here's what to do:

### For Windows Users

Bundle these files together:
- `simplic.exe` (the program)
- `libgcc_s_dw2-1.dll` (helper file from MinGW)
- `libstdc++-6.dll` (another helper file from MinGW)

Your friends can just double-click `simplic.exe` and start coding!

### For Linux/Mac Users

They'll need to compile it themselves, but it's easy:
```bash
g++ -std=c++11 -o simplic main.cpp interpreter.cpp dsa.cpp value_impl.cpp
```

Then they can run it with: `./simplic`

## Who is SimpliC For?

SimpliC is perfect if you're:
- A student learning data structures for the first time
- A teacher looking for an easy way to explain algorithms
- Someone who wants to test algorithm ideas quickly
- Experimenting with data structures
- Learning to code and finding other languages too confusing

Basically, if you want to understand *how* algorithms work without getting lost in syntax, SimpliC is for you!

## Running Into Problems?

Don't worry, here are solutions to common issues:

### "I'm getting a 'Missing DLL' error!" (Windows)

**The Fix:** Those helper files (`libgcc_s_dw2-1.dll` and `libstdc++-6.dll`) need to be in the same folder as `simplic.exe`. Copy them from your MinGW installation!

### "I'm getting syntax errors!"

**Check these things:**
- Did you wrap your code in `start program` and `end program`?
- Are your strings in double quotes? Like `"Hello"` not `'Hello'`
- Are you using the right operator words? It's `plus` not `+`, `times` not `*`

### "Nothing is showing up!"

**Try this:**
- Make sure you're using `show` to print things
- Double-check the path to your `.simplic` file
- Try running the example: `simplic.exe example.simplic`

## Want to Help Make SimpliC Better?

I'd love your help! Here's how you can contribute:

- Found a bug? Let me know!
- Have an idea for a cool feature? Share it!
- See something confusing in the docs? Help make it clearer!
- Want to add something? Send a pull request!

Every contribution, big or small, is appreciated!

## License

SimpliC is free and open source! It's released under the MIT License, which means you can use it for pretty much anything you want. Learn with it, teach with it, modify it - go wild!

## Try the Example Program

Want to see everything SimpliC can do? Check out the example file:

```bash
simplic.exe example.simplic
```

This program shows off:
- Variables and math
- Lists and sorting
- Stacks and queues
- Binary search trees
- Functions and recursion
- If-else statements

It's like a tour of all SimpliC's features!

## About This Project

I created SimpliC because I wanted to make learning data structures and algorithms less scary. Programming languages can be really confusing when you're just starting out - all those symbols and weird syntax rules!

SimpliC uses plain English words so you can focus on understanding *how* things work, not *what* the syntax means. It's a learning tool, not a production language, and that's exactly what makes it useful.

I hope SimpliC helps you on your coding journey!

---

**Happy Coding! Let's make learning fun!**

*Made with care by someone who remembers how hard learning to code can be*
