<div align="center">

# to-9km-and-beyond 
## Time to Climb Project (Wing Optimization for a Subsonic Aircraft)
## by Kaden Dadabhoy

</div>

<br>

## Overview and Coding Methodology

### Main Objective 
The main objective is to optimize a wing to get an aircraft with given engine and given min and max wieght to 9km
(fill in later)
<br>



### Coding Methodology Overview
A secondary goal of the project was to have the program be robust. Robust meaning that any component could be easily modified - in a safe manner - by any user familar with C++. Classes (OOP) were used to accomplish this goal. 

Every Airplane has a mainWing, a Horizontal Tail, a Vertical Tail, Engine(s), Nacelle(s), a Fuselage, a fuel weight, and a payload weight. So, each of the corresponding main components (everything that isn't a single float or integer variable) have their own classes. (Note: these classes are not derived from Airplane, because they a "has-a," not an "is-a" relationship).

Note: It could be helpful to know, now, that in the current Airplane Class implementation every Airplane has two identical engines and two identical nacelles.



### How Object Oriented Programming (OOP) Enables Robustness w/ Examples  
#### (Feel Free to  Skip if Already Familiar with OOP)
This methodology, as those familiar with OOP already know, enables the Airplane class to treat each of these components (class objects) as a "black box." In other words, it does not matter, from the Airplane Class's point of view, how the Wing (for example) object is implemented, just that we have a type Wing, and we can call on it's public member functions. Again, as those familiar with OOP already know, this is an incredibly robust approach, since we are now able to change the implementation of major components, without having to change anything in the Airplane class, as long as the same functions still exist. It is incredibly hard to underscore how useful this is.

For example, if we ever wanted to code an Airplane to go supersonically, we would just need to modify the classes who's implementation changes based on if an an aircraft is subsonic or supersonic (namely the Drag Class).

Another example, is currently the Wing class operates under the assumption that every wing is trapezodial. The airplane class doesn't care about these assumptions, just that it has a Wing object that can call on certain (neccessary) public member functions that the Airplane Class's implementation may need. So, if we ever wanted to run this program with a non-trapezodial wing, the only thing we would have to change is the implementation of Wing Class to be able to handle trapezodial Wings. (Note: One approach (polymorphism) is making the Wing class a base class and deriving a trapezodial wing class from the Wing Class, and then deriving the other type of wing from the Wing Class, which is a bit more advanced than just changing the memeber functions of the Wing Class, but is still in essence, just changing the implementaiton of the Wing Class). 

The cost of the robustness gained from using this methodology, is the time and effort it takes to implement the classes. Each class, at the bare minimum, needs a constructor, accessors, and mutators (if it is dynamic it needs copy constructors, assignment operators, etc).

This cost is worth it for me, as will be explained in "Why C++."




### Why C++
When I first started to brainstorm and outline how I wanted to do this project, I realized it would be a lot more readable if an OOP language was used. This is due to the fact that having classes is very desierable for this program (especially for it's readability). Take a moment to try and think of how to make a robust program without using classes for the major components of the Airplane... In my opinion, any program developed for this project, not using OOP, will be significantly less robust, less readable, and frankly a lot more annoying to code. Additionally, I realized the need (or at least desire) to have an efficient program. This desirability for efficiency is due to the fact that since there would be many classes, many objects would be created - especially in the optimiziation portion. 

When I started outlining how I wanted to implement each function I would need (or at least the key functions), and then figured out the classes I would need to develop, 
I started to realize the magnitude of how many objects would be created, passed, or used. Additionally, since the goal is to optimize an object (the mainWing)


C++ is a low level, OOP, programming language, which enables more efficiency than say Python. Personally, I am also more familiar with C++ than any other programming language, since most of the courses I have taken for my 
Information and Computer Science Minor, have been taught in C++. The cost of getting this extra efficiency is that almost everything had to be built from the ground up (like kadenMath functions, which a language like Python might already have a library for).

This project really benefits from being efficient. When I started outlining how I wanted to implement each function I would need (or at least the key functions), and then figured out the classes I would need to develop, 
I started to realize the magnitude of how many objects would be created, passed, or used. Additionally, since the goal is to optimize an object (the mainWing) , it just made a lot of since to use a language known for efficiency. 

Therefore, for the efficiency, and because I am most comfortable coding in C++, C++ was used to develop this program. 

(Note: this program was developed on Windows, on an x64 device, and no effort was put into size_type stuff... so it is best to use this program on a Windows x64 device).





### A Quick Word on Reusability
By programming using the aforementioned methodologies, the classes I developed in this project are, in many ways black boxes. (An aside, some of the classes depend on other classes in this program, so they are not true black boxes... but they are pretty close.) Therefore, these classes are very re-usable, which is valuable to me because it saves me the time and effort of developing them for my other aero-related personal projects. Additionally, I developed each class to be very readable (to a person familar with C++), which makes it easier to add to or change their respective implementations. This was another compelling reason to develop this program in C++.



## Classes and Assumptions in Each Class:

<br>

### Airfoil:
#### Overview:
#### Assumptions:

<br>

### Airplane:
#### Overview:

#### Assumptions:
1. abc
2. abcc
3. abcc

#### Notable Functions:

##### void getPowerCurveCSV(double gamma, double height, string fileName) const
description:

##### double calcBestTimeTo9km(double startHeight, double takeOffEndHeight)
description:

Relies on:
###### double calcSteadyLevelAccelerationTime(double startVelocity, double finalVelocity, double height)
description:

###### double calcBestClimbTime(double startHeight, double startVelocity, double endHeight);        // Returns seconds 
description: This also relies upon other functions




##### void calcAndSetLiftCoeff()
description:

##### void calcAndSetPowerCurveData(double height)
description:


##### void calcAndSetPowerCurveData(double height)
description:





<br>

### AtmosphereProperties:
#### Overview:
#### Assumptions:

<br>

### CF_34_3B1:
#### Overview:
#### Assumptions:

<br>

### CF_34_3B1:
#### Overview:
#### Assumptions:

<br>

### DragCoeff:
#### Overview:
#### Assumptions:


<br>

### Fuselage:
#### Overview:
#### Assumptions:

<br>

### LiftCoeff:
#### Overview:
#### Assumptions:

<br>

### Nacelle:
#### Overview:
#### Assumptions:

<br>

### Wing:
#### Overview:
#### Assumptions:

