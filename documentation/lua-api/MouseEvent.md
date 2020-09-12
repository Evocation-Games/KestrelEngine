###### Kestrel Lua API / Events

# Mouse Events

The `MouseEvent` structure encompasses information about a given occurance of a mouse event, such as button press, release or movement.

## Receiving MouseEvents

It is currently only possible to receive `MouseEvent` through the `Scene:onMouseEvent(Function(MouseEvent))` callback of the currently active scene. You can register a callback with the current scene like so:

```lua
Scene.current:onMouseEvent(function(event)
	-- Handle mouse events in here...
end)
```

It is currently not possible to remove mouse event callbacks from the scene, so it is recommended to avoid registering callbacks arbitrarily. Instead setup a single callback when a scene is created and dispatch the event to the relavent components in your scene manually.


## Instance Properties

The following properties are exposed on `MouseEvent` instances.

| Property Name | Type | Lua API Version |
| --- | --- | --- |
| `point` | `Point` | v0.0.1 |
| `action` | `Integer` | v0.0.1 |
| `button` | `Integer` | v0.0.1 |
| `moved` | `Boolean` | v0.0.1 |
| `isPressed` | `Boolean` | v0.0.1 |
| `isReleased` | `Boolean` | v0.0.1 |

---
### `MouseEvent.point`
Returns a `Point` structure containing the coordinates of the mouse cursor within the `Scene`.

---
### `MouseEvent.action`
Returns a numeric value representing the current action being undertaken by the mouse. It is recommended that you use the `MouseEvent.moved`, `MouseEvent.isPressed` and `MouseEvent.isReleased` properties for this instead, unless you are expecting values that are not covered by the those properties.

---
### `MouseEvent.button`
Returns an integer value representing the button number (if applicable) that is causing the event to occur.

---
### `MouseEvent.moved`
Returns a boolean based upon whether the mouse is currently being moved or not.

---
### `MouseEvent.isPressed`
Returns a boolean based upon whether the event button is being pressed or not.

---
### `MouseEvent.isReleased`
Returns a boolean based upon whether the event button is being released or not.