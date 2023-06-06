# Angle
| Aspect | Value |
| --- | --- |
| Source File | `libKestrel/math/angle.hpp` |
| C++ Symbol | `kestrel::math::angle` |
| Introduced | Version 0.8.0 |
> #### Warning
> This is a test warning
## Description
Represents an angle.
This is used by the physics engine to determine represent the angle between two vectors,
as required by certain calculations (trajectories, movements, etc)
## Example Code
```
local position = Angle(45):vector(100)
```
## Constructor
```
Angle(theta)
```
## Properties

 - [radians](radians.md)
 - [degrees](degrees.md)
## Functions

 - [sin(magnitude)](sin.md)
 - [isOpposing(a, tolerance)](isOpposing.md)
 - [normalize()](normalize.md)
 - [cos(magnitude)](cos.md)
 - [rotated(phi)](rotated.md)
 - [vector(magnitude)](vector.md)
 - [subtractAngularDifference(a)](subtractAngularDifference.md)
 - [addAngularDifference(a)](addAngularDifference.md)
 - [isEqual(a, tolerance)](isEqual.md)
 - [fsin(magnitude)](fsin.md)
 - [opposite()](opposite.md)
 - [fcos(magnitude)](fcos.md)
