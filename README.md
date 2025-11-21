# Time to Climb Project (Wing Optimization for a Subsonic Aircraft)
## to-9km-and-beyond 
## by Kaden Dadabhoy
<br>

## Objectives
Optimize a wing to get an aircraft with given enginge and given min and max wieght to 9km
(fill in later)
<br>

### Coding Metholody (and other Objectives)
#### Overview
A secondary goal of the project was to have the program be robust. Robust meaning that any component could be easily modified - in a safe manner - by any user familar with C++. Classes (OOP) were used to accomplish this goal. 

Every Airplane has a mainWing, a Horizontal Tail, a Vertical Tail, an Engine, a Nacelle, a Fuselage, a fuel weight, and a payloadweight. So, each of the corresponding main components have their own classes. 

<br>
#### How OOP enables Robustness of this program (can skip if already familar with OOP)
This, as those familar with OOP already know, enables the Airplane class to treat each of these components as a "black box." 
In other words, it does not matter, from the Airplane Class's point of view, how the Wing (for example) object is implemented, just that we have a type Wing, and we can call on it's member functions.
Again, as those familar with OOP already know, this is an increbily robust approach, since we are now able to change the implementation of major components, in a common-sense, safe, and simple way.

For example, if we ever wanted to code an Airplane to go supersonically, we would just need to modify the Drag Class (and other implementations that are limited to subsonic). Another example is if we
ever wanted to change from a trapezodial wing assumption, all we need to do is change the implementation of the Wing Class (or create a derived class from Wing).

The cost of using OOP and this robustness is the time and effort it takes to implement the classes. Each class, at the bare minimum, needs a constructor, accessors, and mutators (if it is dynamic it needs copy constructors, assignment operators, etc).

This cost is worth it 

<br>
#### Why C++:
C++ is a low level, OOP, programming languge, which enables more efficiency than say Python. Personally, I am also more familar with C++ than any other programming language, since most of the courses I have taken for my 
Information and Computer Science Minor, have been taught in C++. The cost of getting this extra efficiency is that almost everything had to be built from the ground up (like kadenMath functions, which a language like Python might already have).

This project really benefits from being efficient. When I started outlining how I wanted to implement each function I would need (or at least the main ones), and then figured out the classes I would need to create, 
I started to realize the magnitude of how many objects would be created, passed, or used. Additionally, since the goal is to optimize an object (or multiple objects), it just made a lot of since to use a language
known for efficiency. 

Therefore, for the efficiency, and because I am most comfortable coding in C++, C++ was used to develop this program.
(Note this program was developed on Windows, on an x64 device, and no effort was put into size_type stuff... so it is best to use this program on a Windows x64 device).

<br>
#### Classes:

<br>
#### A Quick Word on Resuability
By programming using the aforementioned metholodiges, particularly the classes I developed when creating this project, I am also able to re-use these classes for some of my other aero-related personal project (since these classes are, in many ways, black boxes).
(An aside, some of the classes depend on other classes in this program, so they are not true black boxes... but they are pretty close). This was another reason why the program was developed for robustness.

<br>
## Assumptions (per Class)

