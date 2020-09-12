###### Kestrel Lua API / Legacy Assets / Images

# Macintosh ColorIcon

The `MacintoshColorIcon` class is used to load, and manage a `cicn` resource, and from which an `Entity` can be spawned.

Macintosh ColorIcons are one of the _Legacy Assets_ (as in they are a Legacy format, not legacy code) and a fundamental image format supported by Kestrel. They can represent small single frame images, otherwise referred to as icons.

Be sure to read the documentation of _Image Assets and Entities in Kestrel_ to get a better understanding of how Kestrel handles image assets.

## Loading an icon

Loading a `cicn` resource (indeed any image asset) is a trivial task in Kestrel. However there are 2 methods of accomplishing this, depending on whether you want to have asset caching or not.

###### Without Caching

```lua
-- Load the cicn of #128
local icon = MacintoshColorIcon(Resource.id(128))
```

###### With Caching

```lua
-- Load the cicn of #128
local icon = MacintoshColorIcon.load(Resource.id(128))
```

You'll almost always want to use the second option, to avoid having to redecode the icon everytime you request it. However if you have an icon that appears only once, or infrequently you may opt to use the first option.

## Static Methods
The following methods are exposed on the `MacintoshColorIcon` class.

| Method Name | Return Type | Lua API Version |
| --- | --- | --- |
| `load(ResourceReference)` | `MacintoshColorIcon ` | v0.0.1 |

---
### `MacintoshColorIcon.load(ResourceReference)`
Returns a reference to the requested `MacintoshColorIcon ` from cache, or if the requested icon could not be found, loads and caches the icon before returning a reference to the caller.

##### Example
```lua
local icon = MacintoshColorIcon.load(Resource.id(128))
```

## Instance Properties

The following properties are exposed on `MacintoshColorIcon`.

| Property Name | Type | Lua API Version |
| --- | --- | --- |
| `size` | `Size` | v0.0.1 |


---
### `MacintoshColorIcon.size`
Returns a structure that contains the width and the height of the icon.


## Instance Methods
The following methods are exposed on the `MacintoshColorIcon` instances.

| Method Name | Return Type | Lua API Version |
| --- | --- | --- |
| `spawnEntity(Vec2)` | `Entity` | v0.0.1 |

---
### `MacintoshColorIcon:spawnEntity(Vec2)`
Returns a new `Entity` instance at the specified coordinates. The entity is configured to use the icon as its sprite sheet.

##### Example
```lua
local alertIndicator = alertIcon:spawnEntity(Vec2(0, 0)) -- alertIndicator is an Entity

scene:render(function()
	alertIndicator:draw()
end)
```