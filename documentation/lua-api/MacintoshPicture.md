###### Kestrel Lua API / Legacy Assets / Images

# Macintosh Picture

The `MacintoshPicture` class is used to load, and manage a `PICT` resource, and from which an `Entity` can be spawned.

Macintosh Pictures are one of the _Legacy Assets_ (as in they are a Legacy format, not legacy code) and a fundamental image format supported by Kestrel. They can represent single frame images, or complex sprite sheets.

Be sure to read the documentation of _Image Assets and Entities in Kestrel_ to get a better understanding of how Kestrel handles image assets.

## Loading a PICT

Loading a PICT resource (indeed any image asset) is a trivial task in Kestrel. However there are 2 methods of accomplishing this, depending on whether you want to have asset caching or not.

###### Without Caching

```lua
-- Load the PICT of #128
local pict = MacintoshPicture(Resource.id(128))
```

###### With Caching

```lua
-- Load the PICT of #128
local pict = MacintoshPicture.load(Resource.id(128))
```

You'll almost always want to use the second option, to avoid having to redecode the image everytime you request it. However if you have an image that appears only once, or infrequently you may opt to use the first option.

## Static Methods
The following methods are exposed on the `MacintoshPicture` class.

| Method Name | Return Type | Lua API Version |
| --- | --- | --- |
| `load(ResourceReference)` | `MacintoshPicture` | v0.0.1 |

---
### `MacintoshPicture.load(ResourceReference)`
Returns a reference to the requested `MacintoshPicture` from cache, or if the requested picture could not be found, loads and caches the picture before returning a reference to the caller.

##### Example
```lua
local pict = MacintoshPicture.load(Resource.id(128))
```

## Instance Properties

The following properties are exposed on `MacintoshPicture`.

| Property Name | Type | Lua API Version |
| --- | --- | --- |
| `size` | `Size` | v0.0.1 |
| `numberOfSprites` | `Integer` | v0.0.1 |


---
### `MacintoshPicture.size`
Returns a structure that contains the width and the height of an individual sprite within the image. If the image only consists of a single sprite, then this will be the size of the image.

---
### `MacintoshPicture.numberOfSprites`
Returns a count for the number of sprites that the image contains. This is calculated internally based upon the underlying size of the image vs the size of an individual sprite, and can not be directly specified.

## Instance Methods
The following methods are exposed on the `MacintoshPicture` instances.

| Method Name | Return Type | Lua API Version |
| --- | --- | --- |
| `spawnEntity(Vec2)` | `Entity` | v0.0.1 |
| `setSpriteSize(Size)` | None | v0.0.1 |

---
### `MacintoshPicture:spawnEntity(Vec2)`
Returns a new `Entity` instance at the specified coordinates. The entity is configured to use the image as its sprite sheet.

##### Example
```lua
local ship = battleCruiserSprites.spawnEntity(Vec2(0, 0)) -- ship is an Entity

scene:render(function()
	ship:draw()
end)
```

---
### `MacintoshPicture:setSpriteSize(Size)`
Specifies the size of a single sprite within the image.