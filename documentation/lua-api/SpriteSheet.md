###### Kestrel Lua API / Assets / Images

# Sprite Sheet

The `SpriteSheet` class is used to load, and manage a `SpSh` resource, and from which an `Entity` can be spawned.

Sprite Sheets (as in `SpSh`, not the general sprite sheet concept) are a modern format introduced in Kestrel. For more information about the binary format of this resource please read the documentation of _Kestrel Sprite Sheets_. This format is a container format containing both metadata and a Targa (TGA) sprite sheet image.

Be sure to read the documentation of _Image Assets and Entities in Kestrel_ to get a better understanding of how Kestrel handles image assets.

## Loading a Sprite Sheet

Loading a `SpSh` resource (indeed any image asset) is a trivial task in Kestrel. However there are 2 methods of accomplishing this, depending on whether you want to have asset caching or not.

###### Without Caching

```lua
-- Load the SpSh of #128
local sheet = SpriteSheet(Resource.id(128))
```

###### With Caching

```lua
-- Load the SpSh of #128
local sheet = SpriteSheet.load(Resource.id(128))
```

You'll almost always want to use the second option, to avoid having to redecode the sprite sheet everytime you request it. However if you have an sprite sheet that appears only once, or infrequently you may opt to use the first option.

## Static Methods
The following methods are exposed on the `SpriteSheet` class.

| Method Name | Return Type | Lua API Version |
| --- | --- | --- |
| `load(ResourceReference)` | `MacintoshPicture` | v0.0.1 |

---
### `SpriteSheet.load(ResourceReference)`
Returns a reference to the requested `SpriteSheet ` from cache, or if the requested sprite sheet could not be found, loads and caches the sprite sheet before returning a reference to the caller.

##### Example
```lua
local sheet = SpriteSheet.load(Resource.id(128))
```

## Instance Properties

The following properties are exposed on `SpriteSheet` instances.

| Property Name | Type | Lua API Version |
| --- | --- | --- |
| `size` | `Size` | v0.0.1 |
| `numberOfSprites` | `Integer` | v0.0.1 |


---
### `SpriteSheet.size`
Returns a structure that contains the width and the height of the first sprite in the sprite sheet.

_Warning: It is not recommended to purely rely on this value, as it can have unintended consequences._

---
### `SpriteSheet.numberOfSprites`
Returns a count for the number of sprites that the image contains.

## Instance Methods
The following methods are exposed on the `SpriteSheet` instances.

| Method Name | Return Type | Lua API Version |
| --- | --- | --- |
| `spawnEntity(Vec2)` | `Entity` | v0.0.1 |

---
### `MacintoshPicture:spawnEntity(Vec2)`
Returns a new `Entity` instance at the specified coordinates. The entity is configured to use the receiver as its sprite sheet.

##### Example
```lua
local ship = battleCruiserSprites.spawnEntity(Vec2(0, 0)) -- ship is an Entity

scene:render(function()
	ship:draw()
end)
```