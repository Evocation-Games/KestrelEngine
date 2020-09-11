###### Kestrel Lua API / Scene Presentation

# Scene

The `Scene` class represents a given "scene" within Kestrel. This may be a scene that you wish to move to/present to the user or the currently presented scene.

Scenes are how Kestrel handles all contextual state. An overworld view of the game may be its own scene, whilst an NPC communication view maybe another scene. As scenes are presented, they are placed upon a stack, with Kestrel rendering and sending events only the top most scene on the stack.

## Presenting a Scene
In order to present a scene, you will need to have a _Controller_ script to manage the functionality of the scene. Such scripts are encoded as `LuaC` resources. Below are example KDL and the Lua script files for this illustration.

```lua
-- scripts/MyScene.lua

print("Hello, World")
```

```kdl
` game.kdl

declare LuaScript {
    new (#128) {
        Script = import "@rpath/scripts/MyScene.lua";
    };
};

```

This scene can then be loaded and presented using the following snippet.

```lua
local scene = Kestrel.scene("Example Scene", Resource.id(128))
scene:present()
```

Here, we're asking Kestrel to create a new scene with an internal identifier of `Example Scene`. This identifier is purely for our own, debugging purposes and is not related to the underlying resource data or controller script. The `Resource.id(128)` is telling the scene that the controller script for it, is in resource 128. The `LuaC` type in this instance is inferred.


## Acquiring the current Scene
It will almost certainly be likely that you need to acquire the current scene that Kestrel is presenting. Whether this be within the confines of the controller script, or in another aspect of functionality that you have added.

You can acquire the current scene like so

```lua
local scene = Scene.current()
```


## Direct Instantation
Due to the internal structure of Kestrel and how deeply integrated Scenes are in the architecture of Kestrel, it is not possible to directly instantiate a scene currently. You must go through the `Kestrel.scene` method.

## Static Methods
The following methods are exposed on the `Scene` class.

| Method Name | Return Type | Lua API Version |
| --- | --- | --- |
| `current` | `Scene` | v0.0.1 |

---
### `Scene.current`
Returns a reference to the scene that is currently at the top of the scene stack in Kestrel.

##### Example
```lua
local scene = Scene.current()
```

## Instance Methods
The following methods are exposed on `Scene`.

| Method Name | Return Type | Lua API Version |
| --- | --- | --- |
| `present` | None | v0.0.1 |
| `render` | None | v0.0.1 |
| `onKeyEvent` | None | v0.0.1 |
| `onMouseEvent` | None | v0.0.1 |


---
### `Scene:present`
Pushes the specified scene on to Kestrels internal scene stack, causing the specified scene to become the current scene.

##### Example
```lua
local scene = Kestrel.scene("Example Scene", Resource.id(128))
scene:present()
```

---
### `Scene:render`
Provide a function to the scene that will be called each time a new render pass is made. You should perform any updates, and drawing of entities in these functions/closures.

##### Example
```lua
scene:render(function()
	ball:draw()
end)
```

---
### `Scene:onKeyEvent`
Provide a function to the scene that will handle keyboard events.

##### Example
```lua
scene:onKeyEvent(function(event)
	if event.keycode == 262 and event.pressed then
		print("Left Button pressed")
	end
end)
```

---
### `Scene:onMouseEvent`
Provide a function to the scene that will handle keyboard events.

##### Example
```lua
scene: onMouseEvent(function(event)
	if event.moved then
		local position = event.point
		print("Mouse moved")
	end
end)
```


