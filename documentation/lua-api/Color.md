###### Kestrel Lua API / Graphics

# Color

The `Color` structure encompasses information about a given color, as well as providing conveinence functions for constructing common colors.

## Constructing Colors

The `Color` structure is a fundamental structure within the Kestrel Engine, but doesn't currently serve much purpose within the Lua API. To generate a `Color` structure you have two choices. Manually construct it, or use a premade common color.

#### Manual Construction
When manually constructing a color, you are specifying the _red_, _green_, _blue_ and _alpha_ components of the color, and these values are expected to be values ranging from `0.0` to `1.0`. Kestrel will then handle the scaling of this value depending on the context and color space.

```lua
local textColor = Color(1.0, 0.0, 0.5, 1.0) -- a purplish color
```

#### Common Color
Kestrel provides a number of premade colors for use. Whilst the vast majority of these are unlikely to de used for production purposes, they can serve nicely for debug purposes.

```lua
local textColor = Color.orange()
```

## Static Methods
The following methods are exposed on the `Color` class.

| Method Name | Return Type | Lua API Version |
| --- | --- | --- |
| `whiteColor(Integer, Integer)` | `Color` | v0.0.1 |
| `rgb(Integer, Integer, Integer, Integer)` | `Color` | v0.0.1 |
| `colorValue(Integer)` | `Color` | v0.0.1 |
| `white()` | `Color` | v0.0.1 |
| `lightGrey()` | `Color` | v0.0.1 |
| `grey()` | `Color` | v0.0.1 |
| `darkGrey()` | `Color` | v0.0.1 |
| `black()` | `Color` | v0.0.1 |
| `red()` | `Color` | v0.0.1 |
| `orange()` | `Color` | v0.0.1 |
| `yellow()` | `Color` | v0.0.1 |
| `lime()` | `Color` | v0.0.1 |
| `green()` | `Color` | v0.0.1 |
| `teal()` | `Color` | v0.0.1 |
| `blue()` | `Color` | v0.0.1 |
| `magenta()` | `Color` | v0.0.1 |

---
### `Color.whiteColor(Integer, Integer)`
Returns a new greyscale `Color` structure using the specified white intensity value (`0` to `255`) and the alpha value (`0` to `255`).

---
### `Color.rgb(Integer, Integer, Integer, Integer)`
Returns a new RGB `Color` structure using the specified red intensity value (`0` to `255`), green intensity value (`0` to `255`), blue intensity value (`0` to `255`) and the alpha value (`0` to `255`).

---
### `Color.colorValue(Integer)`
Returns a new RGB `Color` structure using the specified 32bit color value (format of `0xAARRGGBB`) as a basis for each of the component values.

---
### `Color.white()`
Returns a white color.

---
### `Color.lightGrey()`
Returns a light grey color.

---
### `Color.grey()`
Returns a grey color.

---
### `Color.darkGrey()`
Returns a dark grey color.

---
### `Color.black()`
Returns a black color.

---
### `Color.red()`
Returns a red color.

---
### `Color.orange()`
Returns an orange color.

---
### `Color.yellow()`
Returns a yellow color.

---
### `Color.lime()`
Returns a lime color.

---
### `Color.green()`
Returns a green color.

---
### `Color.teal()`
Returns a teal color.

---
### `Color.blue()`
Returns a blue color.

---
### `Color.magenta()`
Returns a magenta color.


## Instance Properties

The following properties are exposed on `Color` instances.

| Property Name | Type | Access | Lua API Version |
| --- | --- | --- | --- |
| `red` | `Integer ` | Set, Get | v0.0.1 |
| `green` | `Integer` | Set, Get | v0.0.1 |
| `blue` | `Integer` | Set, Get | v0.0.1 |
| `alpha` | `Integer ` | Set, Get | v0.0.1 |

---
### `Color.red`
Returns or sets the red component value for the color.

---
### `Color.green`
Returns or sets the green component value for the color.

---
### `Color.blue`
Returns or sets the blue component value for the color.

---
### `Color.alpha`
Returns or sets the alpha component value for the color.