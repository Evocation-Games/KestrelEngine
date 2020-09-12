###### Kestrel Lua API / Math

# Point

The `Point` structure encompasses information about a given point in a coordinate system, and can be used to perform various operations and calculations regarding distances between two points in space.

## Making a Point

Constructing a new point is a trivial operation. All you need to do is simply:

```lua
local p = Point(50, 100)
```

The first value specified is the X-coordinate, and the second is the Y-coordinate.

## Instance Properties

The following properties are exposed on `Point` instances.

| Property Name | Type | Access | Lua API Version |
| --- | --- | --- | --- |
| `x` | `Double ` | Set, Get | v0.0.1 |
| `y` | `Double` | Set, Get | v0.0.1 |

---
### `Point.x`
Returns or sets the X-coordinate value for the point.

---
### `Point.y`
Returns or sets the Y-coordinate value for the point.

## Instance Methods
The following methods are exposed on `Point` instances.

| Method Name | Return Type | Lua API Version |
| --- | --- | --- |
| `distanceTo(Point)` | `Double` | v0.0.1 |
| `subtract(Point)` | `Point` | v0.0.1 |
| `add(Point)` | `Point` | v0.0.1 |
| `multiply(Double)` | `Point` | v0.0.1 |
| `divide(Double)` | `Point` | v0.0.1 |

---
### `Point.distanceTo(Point)`
Calculates the distance between the receiver and the specified point and returns the value as a `Double`.

##### Example
```lua
Point(1, 1):distanceto(Point(5, 1)) -- returns 4.0
```

---
### `Point.subtract(Point)`
Subtracts the specified point from the receiver, resulting in a new point.

##### Example
```lua
Point(5, 5):subtract(Point(5, 1)) -- returns Point(0, 4)
```

---
### `Point.add(Point)`
Add the specified point to the receiver, resulting in a new point.

##### Example
```lua
Point(5, 5):add(Point(5, 1)) -- returns Point(10, 6)
```

---
### `Point.multiply(Double)`
Multiplies/Scales the receiver by the specified value, resulting in a new point.

##### Example
```lua
Point(1, 0.5):multiply(10) -- returns Point(10, 5)
```

---
### `Point.divide(Double)`
Divides/Scales the receiver by the specified value, resulting in a new point.

##### Example
```lua
Point(10, 5):divide(2) -- returns Point(1, 0.5)
```