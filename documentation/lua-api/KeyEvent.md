###### Kestrel Lua API / Events

# Keyboard Events

The `KeyEvent` structure encompasses information about a given occurance of a keyboard event, such as key press, hold or release.

## Receiving KeyEvents

It is currently only possible to receive `KeyEvents` through the `Scene:onKeyEvent(Function(KeyEvent))` callback of the currently active scene. You can register a callback with the current scene like so:

```lua
Scene.current:onKeyEvent(function(event)
	-- Handle key events in here...
end)
```

It is currently not possible to remove key event callbacks from the scene, so it is recommended to avoid registering callbacks arbitrarily. Instead setup a single callback when a scene is created and dispatch the event to the relavent components in your scene manually.


## Instance Properties

The following properties are exposed on `KeyEvent` instances.

| Property Name | Type | Lua API Version |
| --- | --- | --- |
| `keycode` | `Integer` | v0.0.1 |
| `scancode` | `Integer` | v0.0.1 |
| `shift` | `Boolean` | v0.0.1 |
| `alt` | `Boolean` | v0.0.1 |
| `control` | `Boolean` | v0.0.1 |
| `super` | `Boolean` | v0.0.1 |
| `caps` | `Boolean` | v0.0.1 |
| `num` | `Boolean` | v0.0.1 |
| `pressed` | `Boolean` | v0.0.1 |
| `held` | `Boolean` | v0.0.1 |
| `released` | `Boolean` | v0.0.1 |


---
### `KeyEvent.keycode`
Returns a numeric value representing the character that resulted from the key that triggered the event. This is not the same as `KeyEvent.scancode`.

---
### `KeyEvent.scancode`
Returns a numeric value representing the physical key that triggered the event. This is not the same as `KeyEvent.keycode`.

---
### `KeyEvent.shift`
Returns a boolean based upon whether the shift modifier key was being pressed or not.

---
### `KeyEvent.alt`
Returns a boolean based upon whether the alt modifier key was being pressed or not.

---
### `KeyEvent.control`
Returns a boolean based upon whether the control modifier key was being pressed or not.

---
### `KeyEvent.super`
Returns a boolean based upon whether the super modifier key was being pressed or not.

---
### `KeyEvent.caps`
Returns a boolean based upon whether the caps modifier key was being pressed or not.

---
### `KeyEvent.num`
Returns a boolean based upon whether the num lock modifier key was being pressed or not.

---
### `KeyEvent.pressed`
Returns a boolean based upon whether the key has just been pressed.

---
### `KeyEvent.pressed`
Returns a boolean based upon whether the key is being held down.

---
### `KeyEvent.released`
Returns a boolean based upon whether the key has just been released.

