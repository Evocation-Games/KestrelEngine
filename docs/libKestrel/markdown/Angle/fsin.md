[Angle](index)
# fsin(magnitude)
| Aspect | Value |
| --- | --- |
| Source File | `libKestrel/math/angle.hpp` |
| C++ Symbol | `kestrel::math::fsin` |
| Introduced | Version 0.8.0 |
> #### Warning
> This method is less accurate than `Angle:sin()`, but it is faster. Make sure you use the
most appropriate version for your use-case.
## Description
Compute the sine of the angle to the specified magnitude. The sin is determined using a look up table.
This is generally faster than using `Angle:sin()` but is less accurate.
## Parameters
| Parameter | Type | Description |
| --- | --- | --- |
| `magnitude` | `?` | The value that the sine will be multiplied by. |
