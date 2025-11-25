<div align="center">

# to-9km-and-beyond 
## Time to Climb Project (Wing Optimization Program for a Subsonic Aircraft)
## by Kaden Dadabhoy

</div>

<br>

## Overview and Coding Methodology

### Main Objective 
```The main objective is to optimize a wing to get an aircraft with given engine and given min and max wieght to 9km```
(fill in later)


<br>
<br>



### Coding Methodology Overview
```A secondary goal of the project was to have the program be robust.``` Robust meaning that any component could be easily modified - in a safe manner - by any user familar with C++. Classes (OOP) were used to accomplish this goal. 

```Every Airplane has (at least) a mainWing, a Horizontal Tail, a Vertical Tail, Engine(s), Nacelle(s), a Fuselage, a fuel weight, and a payload weight.``` 
So, each of the corresponding main components (everything that isn't a single float or integer variable) have their own classes. (Note: these classes are not derived from Airplane, because they a "has-a," not an "is-a" relationship).

Note: It could be helpful to know, now, that in the current Airplane Class implementation every Airplane has two identical engines and two identical nacelles.


<br>
<br>



### How Object Oriented Programming (OOP) Enables Robustness w/ Examples  
#### (Feel Free to  Skip if Already Familiar with OOP)
```Object Oriented Programming (OOP) essentially allows a programmer to create a new "type".``` Common types are ints, doubles, char, etc. In order to create a new type, a programmer needs to create a class (header and implementation files). This class is the new type. ```When a variable is assigned with the new type, it is known as an object. Objects have access to all the public member functions``` (and/or variables... although that is poor practice), that the class defines. Additionally objects have private member variables, which are intialized in the constructor. There is a LOT more detail behind OOP, like polymorpism, encapsulation, best practices, dynamic classes, constructors, destructors, operator overloading, etc; however, the main takeaways are:

        1. The programmer can create any new type
        2. The programmer can develop any functions for the type
        3. Each object has access to the public member functions
        4. Objects (typically are) very memory intensive, so avoid copying them as much as possible
            - ALWAYS pass them by reference
        5. In many way's classes are black boxes 
            - You don't need to know how the public member functions are implemented, just that they exist


This methodology, as those familiar with OOP already know, enables the Airplane class to treat each of these components (class objects) as a "black box." In other words, it does not matter, from the Airplane Class's point of view, how the Wing (for example) object is implemented, just that we have a type Wing, and we can call on it's public member functions. Again, as those familiar with OOP already know, this is an incredibly robust approach, since we are now able to change the implementation of major components, without having to change anything in the Airplane class, as long as the same functions still exist. It is incredibly hard to underscore how useful this is.

For example, if we ever wanted to code an Airplane to go supersonically, we would just need to modify the classes who's implementation changes based on if an an aircraft is subsonic or supersonic (namely the Drag Class).

Another example, is currently the Wing class operates under the assumption that every wing is trapezodial. The airplane class doesn't care about these assumptions, just that it has a Wing object that can call on certain (neccessary) public member functions that the Airplane Class's implementation may need. So, if we ever wanted to run this program with a non-trapezodial wing, the only thing we would have to change is the implementation of Wing Class to be able to handle trapezodial Wings. (Note: One approach (polymorphism) is making the Wing class a base class and deriving a trapezodial wing class from the Wing Class, and then deriving the other type of wing from the Wing Class, which is a bit more advanced than just changing the memeber functions of the Wing Class, but is still in essence, just changing the implementaiton of the Wing Class). 

```The cost of the robustness gained from using this methodology, is the time and effort it takes to implement the classes.``` Each class, at the bare minimum, needs a constructor, accessors, and mutators (if it is dynamic it needs copy constructors, assignment operators, etc).

This cost is worth it for me, as will be explained in "Why C++."


<br>
<br>



### Why C++
When I first started to brainstorm and outline how I wanted to do this project, I realized it ```would be a lot more readable if an OOP language was used.``` This is due to the fact that having classes is very desierable for this program (especially for it's readability). Take a moment to try and think of how to make a robust program without using classes for the major components of the Airplane... In my opinion, any program developed for this project, not using OOP, will be significantly less robust, less readable, and frankly a lot more annoying to code. Additionally, I realized ```the need (or at least desire) to have an efficient program.``` This desirability for efficiency is due to the fact that since there would be many classes, many objects would be created - especially in the optimiziation portion. 

C++ is a low level, OOP, programming language, which enables more efficiency than say Python. ```Personally, I am also more familiar with C++ than any other programming language, since most of the courses I have taken for my Computer Science Minor, have been taught in C++.``` ```The cost of getting this extra efficiency is that almost everything had to be built from the ground up``` (like kadenMath functions, which a language like Python might already have a library for).

This project really benefits from being efficient. When I started outlining how I wanted to implement each function I would need (or at least the key functions), and then figured out the classes I would need to develop, 
I started to realize the magnitude of how many objects would be created, passed, or used. Additionally, since the goal is to optimize an object (the mainWing) , it just made a lot of since to use a language known for efficiency. 

Therefore, for the efficiency, and because I am most comfortable coding in C++, C++ was used to develop this program. 

(Note: this program was developed on Windows, on an x64 device, and no effort was put into size_type stuff... so it is best to use this program on a Windows x64 device).



<br>
<br>



### A Quick Word on Reusability
By programming using the aforementioned methodologies, the classes I developed in this project are, in many ways black boxes. (An aside, some of the classes depend on other classes in this program, so they are not true black boxes... but they are pretty close.) Therefore, these classes are very re-usable, which is valuable to me because it saves me the time and effort of developing them for my other aero-related personal projects. Additionally, I developed each class to be very readable (to a person familar with C++), which makes it easier to add to or change their respective implementations. This was another compelling reason to develop this program in C++.


<br>
<br>
<br>

# main() function - Optimizing the mainWing

## Overview / Approach:
- Rework to be more of an overview (more laymans terms)

The classes, which will be described in much more detail in the "Classes and Assumptions" section, whole purpose is to make it easier, among other things (readbility, modularity, etc), to code an optimizer. The main function first intializes an Airplane (by first initializing all the objects that compose and airplane) with the specific characteristics given in the project description (for objects like mainWing and Airfoil, which aren't given, we intialize them with realistic values). The main function then passes these objects into the optimizers. The optimizers run a simulation (based on a user defined amount of steps from a user defined minimum to a user defined maximum for that parameter) and return the sorted data (in ascending climb order) in a vector. After the last optimizer is called, the main function then calls a function to export the data into an excel sheet, so the user can visually see it (and keep it).



## Functions:


<br>

## Results:




<br>
<br>






# Classes and Assumptions



## Airfoil:
### Overview:
### Assumptions:

<br>

## Airplane:

### Overview:
The purpose of this class is so that a user is able to easily get desired characteristics of an airplane and have an functional airplane object to use (like for simulations). An airplane object is intialized with three Wing objects (Main Wing, Horizontal Tail, and Vertical Tail), an CF34_3B1 object (the engines), a Nacelle object, a Fuselage object, a fuel weight, and a payload weight. 


### Disclosures:
1. ```The class is designed to be used with standard imperial units (ft, s, lbf, etc) and on Earth (lbf=lbm)```
2. ```The class uses pointers behind the scenes. In fact all objects passed in are by reference, and are subject to modification.```
    - So, do NOT try assigning the same objects (like mainWing) to different airplane objects. It can lead to unexpected behavior and is a headache to debug.

### Major Assumptions 
#### Note: Assumptions from other classes used within this class may not be specified but do apply.
1. The Wing weight is approximated using a method from Raymond 
    - The Class only runs this approx for the mainWing (and only if the mainWing has a weight of 0 when passed in)
2. ```The airplane has two engines``` (numEngines = 2) and ```two identical Nacelles.```
3. The ```engines are identical``` (same thrust, same fuel loss)
4. The ```Wing weight is approximated using a method from Raymond (Cargo/Transport Approx)```
    - Assuming 10% of the mainWing is used for control surfaces
        - PERCENT_CONTROL_SURFACE_AREA = 0.10
    - Additionally, assumes FAR 25 cert:
        - Which means that n = 2.1 + (24000 / MTOW), 2.5 <= n <= 3.8
    - Additionally an advanced composite wing is assumed (aka less weight, higher yield)
        - SMUDGE_FACTOR = .80, which is given by Raymond for composite wings
        - SIGMA_YIELD_COMPOSITE = 101.526 ksi (700 MPa)
            - Rough estimate

    
5. Gravity = 32.2 ft/s (constant)
6. pi = 3.141592653589
7. Gas Constant = 1716 (ft*lb) / (slug*R)
8. ```calcBestTimeTo9km uses a "Takeoff to Steady Level Accelerate (to velocity for maxExcessPower) to Climb to Steady Accelerate (to velocity for maxExcessPower) to Climb, etc." approach.```
    - Why?
        - ```Solving for the best path/trajectory/inputs to climb to 9km the fastest is a very complicated dynamics problem that would be a whole project in and of itself.``` In order to solve for that best path/trajectory/inputs, (most likely) hundreds of simulations would need to be performed for each particular aircraft (when to pitch, how much to pitch by, when to fly straight before climbing, when to not bother about reaching the maxExcessPower velocity, because you are close enough, and that marginal gain isn't worth the time sacrafice, how much time it take's for the control surfaces to act, how much drag those control surfaces are making, etc etc etc). 
        - The cherry on top is that this model would change significantly from airplane to airplane and would be very intensive to calculate. ```The goal primary goal this program was designed to accomplish was to size/optimize a mainWing for this mission... not necessairly get the most accurate simulation time.```

    - Drawbacks of this approach
        - First and foremost, this approach is definitely not the most accurate. ```Changing allowable error variables (particularly the velocity error (allowed difference) for entering/eleaving the steadyLevelAccelerate function) drastically alters the simulated times for the aircraft```
            - ```HOWEVER, while simulation times may drastically change the different aircrafts keep the same relative performance.``` In otherwords, if say a 50ft span wing performs has the quickiest time to climb with certain error variables, that 50ft span wing will also have the quickiest time to climb with different error variables. ```The relative performance of the wing is independent of the error variables, which justifies this approach for optimizaiton of a wing, despite the fluculation in overall times.```
            - Additionally, ```adjusting the error variables tends to DECREASE the time to climb```, not increase it (although there are anolmolies). The quickiest time to climbs I was able to achieve were in the 3 min range. ```Currently the program error variables settings give climb times in the 4-5 min range for optimial wings.```

9. ```calcBestTimeTo9km uses the small angle approx for gamma (flight path angle)```
    - Typically gamma is at most 11 degrees... which isn't a true small angle... but is a good approx for the amount of complexity it saves.
        - ```Note: Coupling between AoA, the Power Curve, and gamma, make solving for gamma difficult and computationally heavy.```
        - The program already has a lot of assumptions built into it, this small angle approx for gamma is fairly insignificant in comparison.
9. Assumes the Vertical Tail makes no lift.
10. ```Assumes liftoff occurs at 1.2*V_stall```
11. ```Assumes 15 deg for takeoff AoA_stall,``` this is actually a conservative estimate.


### Notable Functions:

1. ```double calcDragCoeff(double AoA, double velocity, double Mach, double kinematicViscosity) const;```
    - This function returns the Airplane's total Drag Coefficient at the passed in conditions
2. ```double calcDrag(double AoA, double velocity, double Mach, double kinematicViscosity, double density) const```
    - This function returns the Airplane's total Drag (lbf) at the passed in conditions
3. ```double calcLiftCoeff(double AoA) const;```
    - This function returns the Airplane's total Lift Coefficient for that AoA
4. ```double calcLift(double AoA, double velocity, double density) const;```
    - This function returns the Airplane's total Lift (lbf) at the passed in conditions
5. ```void getPowerCurveCSV(double gamma, double height, string fileName) const;```
    - Returns the data (in a file with the passed in fileName) needed to plot a Power Curve at a specific gamma and height
        - If your file name is "exampleName.csv", then you can open it in Excel, select the three columns, create a scatter plot, and you will be able to visually see the Power Curve
            - Ex: (ADD POWER CURVE PIC)
    - This is an identical plot that returns a Power Curve with the small angle gamma assumption (in other words it just needs a height variable)

6. ```Power Curve Private Helper Functions```
    - ***Very important functions for this project***
    - ```void calcAndSetPowerCurveData(double height), which relies on:```
        - vector<double> calcPowerCurveVelocityData(double height) const
            - vector<double> calcPowerAvailableData(double height) const
                - Depends on the Airplane's engine (thrust curve)
                - Evalutes the engine's thrust curve at that specific height and multiplies that by the velocity to get the powerAvailable data points needed (P_available = Thrust*velocity)
            - vector<double> calcPowerRequiredData(double height) const
                - Depends on the airplane's configuration (Total Drag, Total Lift, Total Weight, etc)
                - Calculates the steadyClimbAoA needed for each velocity (which depends on Lift and Weight), which is then used to calculate the totalDrag required for each velocity (and therefore the total power required at that velocity (P_req = Drag*velocity))
                - This function assumes small angle approx for gamma, there is also a very similar function that does not assume this (but you need to pass gamma into it)
        - ```void calcAndSetMaxExcessPower();```
        - ```double calcExcessPower(double velocity) const;```
        - The implementation details will not be described here (but might be added in header documentation in the future - Winter Break). The usage of the data created in these functions relies on kadenMath functions like maxDistBetweenCurves and evalulateFunction. 
            - Essentially these functions are used to find where the maxExcessPower occurs, find your current excessPower, and do all of this in a useful, readable, effective way.
            - ```It is also important to note that these functions create an evenly spaced Power Curve from .01 to .975 Mach (although the x-axis will be in ft/s), with 1000 steps (data points)... In other words, it is not a continous graph (it is discrete), but with 1000 steps, it is fairly accurate.``` 
                - You can also easily change the private data variable STEPS from 1000 to higher, but I doubt you will notice a signficant difference, and the tradeoff is computation time.

7. ```bool isWingPossible() const;```
    - The goal of this function was to see if the wing would be able to withstand the root bending moment (and possibly takeoff considerations). 
    - This function depends on functions from the Wing Class to calculate the moment of inertia.
    - Three attempts were made at implementing this function with three different methods (all within the code, 2/3 commented out) - mainly differing in how they approx the inertia at the root of the wing. The results of these implementations were fairly useless (some methods gave unreasobably high estimates, some gave unreasonable low estimates)
        - A MIT Lab had an emperical equation for intertia that involved using airfoil characteristics (which this class is robust enough to implement)
        - A method of calculating the inertia caused by the caps in the wing (assuming their characteristics and scaling based on rootChord)
            - Multiple Airplane design books (structure design mainly) made claims along the lines that the caps account for most of the inertia in the wing.
        - A method that assumed the internal structure was a box and calculating the inertia of a thin walled box.

8. ```TakeoffProperties calcEndRunwayAirplaneProperties(double height, double startVelocity, double startWeight) const;```
    - ***One of the important function for the purposes of this project***
    - ```Assumes 15 deg for takeoff AoA_stall, this is actually a conservative estimate.```
    - Assumes rolling resistance = .02 (fair assumption)
    - ```Essentially calcs the maxAcceleration = (((thrust - drag - rollingFriction) * GRAVITY) / weight), and then time steps until the plane gets to 1.2V_stall.```
    - ```The function then returns a struct that contains useful propertities (final velocity, final weight, and time taken)```

9. ```double calcBestClimbTime(double startHeight, double startVelocity, double endHeight);```
    - ***One of the important function for the purposes of this project***
    - Uses a "Takeoff to Steady Level Accelerate (to velocity for maxExcessPower) to Climb to Steady Accelerate (to velocity for maxExcessPower) to Climb, etc." approach. And assumes small angle approx for gamma.
        - Justified above
    - ```While the height is below the end height, the function will use that climb logic```
        - ```Calculates the Power Curve for each height uses the Power Curve private helper functions. ```
        - ```If velocity < velocity maxExcess power```
            - ```Call on calcSteadyLevelAccelerationTime() to get to the velocity for maxExcessPower```
                - There is a safeguard built into it, incase the airplane can't reach that velocity (which without this safeguard would cause an infinite loop)
        - ```Else, calculate the change in height with the current properties for a time step. ```
        - There is a safeguard built into it, incase the airplane can't reach the maxHeight (which without this safeguard would cause an infinite loop)
            - The time to climb would be set to 1e10 if the airplane can't reach the endHeight
    - Time taken for steadyLevelAcceleration and climb are taken into account. Weight loss is also taken into account.
    - Returns the climbTime in seconds.


10. ```double calcBestTimeTo9km(double startHeight, double takeOffEndHeight);```
    - ***The most important function for the purposes of this project***
    - This function calls on the calcTakeoffPropertites() and calcBestClimbTime() to get the total amount of time it takes for the Airplane to climb to 9km from rest on a runway (assuming it can instantly have max power in the engines).
    - This function is one that is called 

11. Typical Accessors (but not for every private data memeber) are also available
    - Namely for weights 




<br>
<br>
<br>

## AtmosphereProperties:

### Overview:
The purpose of this class is enable the calculations of AtmosphereProperties in a simple manner. ```A user will be able to create an object, set the height of that object, and instantly be able to get the atmosphere properties (imperial units) with basic function calls on the object.``` In general, and particularly because of the amount of classes in this project, this is extermely beneficial, for readability and removing excess code (without this class you'd need to implement these functions in every class that you would need them in). 

### Disclosures / Assumptions:
1. This class ```relies on the NASA emperical equations``` for the troposphere and stratosphere
    - ```This class will only work accurately in the troposphere and stratosphere```
        - ```Troposphere is from 0ft - 36152 ft```
        - ```Stratosphere is from 36152ft - 82345ft```
2. The Sutherland equation is used for viscosity calculation
3. ```Again this class only works with imperial units```
    - It takes in feet 
    - It returns the imperial unit you would expect... if unsure, just look at the header comment.


### Notable Functions:

1. ```void setHeight(double inHeight);```
    - This function recalculates all the atmosphere properties of the object for the new height.
    - A lot of the time you will just initialize the object with the height and not need to call this function, but there are a handful of times this mutator is useful.
2. ```double getDensity() const;```
    - Returns Density in slug/ft^3
3. ```double getViscosity()  const;```
    - Returns Viscosity in slug/(ft*s)

4. ```double getKinematicVisc()  const;```
    - Returns Kinematic Viscosity in ft^2/s
    
5. ```double getStaticPressure()  const;```
    - Returns Static Pressure in lb/ft^2 

6. ```double getTemperature()  const;```
    - Returns Temperature in deg R

7. ```double getSpeedOfSound()  const;```
    - Returns SpeedOfSound in ft/s

8. ```double getHeight()  const;```
    - Returns height in ft (not sure when you'd need this, but there for modularity)

9. ```Note: There are also calc"WantedProperty" for all the properties there are getters for.```
    - Use setHeight() mutator if you want to re-calc all properties... it is more efficient and recommended (over say using the calc functions for every property).


<br>
<br>
<br>


## CF_34_3B1:

### Overview:

### Assumptions:

### Notable Functions:

1. ```void ;```
    - This function



<br>
<br>
<br>

## DragCoeff:

### Overview:

### Assumptions:

### Notable Functions:

1. ```void ;```
    - This function



<br>
<br>
<br>

## Fuselage:

### Overview:

### Assumptions:

### Notable Functions:

1. ```void ;```
    - This function



<br>
<br>
<br>

## LiftCoeff:

### Overview:

### Assumptions:

### Notable Functions:

1. ```void ;```
    - This function



<br>
<br>
<br>

## Nacelle:

### Overview:

### Assumptions:

### Notable Functions:

1. ```void ;```
    - This function


<br>
<br>
<br>

## Wing:

### Overview:

### Assumptions:


### Notable Functions:

1. ```void ;```
    - This function


