###### Kestrel Lua API / Resource Manager

# ResourceData

The `ResourceData` class can be used to read and parse the data found in any given resource from a resource file (i.e. Classic, Extended and Rez files).

## Instantiation

In order to instantiate any given resource you will need to supply a `Resource` reference during instantiation of the `ResourceData` object. An example of how this is achieved is illustrated below.

```lua
-- Load data for Description Text #128.
local res = ResourceData(Resource.using_typed_id("dësc", 128))
```

Instantiation is guaranteed to work, but valid resource data is not. In order to assert that you have received valid data, you should do the following:

```lua
if res.isValid {
	-- Handle the reading of the resource.
}
```

## Instance Properties

The following properties are exposed on `ResourceData`.

| Property Name | Type | Lua API Version |
| --- | --- | --- |
| `isValid` | Boolean | v0.0.1 |
| `id` | Integer | v0.0.1 |
| `name` | String | v0.0.1 |


---
### `ResourceData.isValid`
Returns a truthy value if the object contains valid resource data for reading. A falsey value will typically only happen if the resource could not be found.

##### Example
```lua
-- Load data for Description Text #128.
local res = ResourceData(Resource.using_typed_id("dësc", 128))
if res.isValid {
	-- Handle the reading of the resource.
}
```

---
### `ResourceData.id`
Returns the numeric id of the resource that was loaded.

##### Example
```lua
-- Load data for Description Text #128.
local res = ResourceData(Resource.using_typed_id("dësc", 128))
local id = res.id -- Returns 128
```

---
### `ResourceData.name`
Returns the name of the resource that was loaded. If the resource has no name, then an empty string will be returned.

##### Example
```lua
-- Load data for Description Text #128.
local res = ResourceData(Resource.using_typed_id("dësc", 128))
local name = res.name -- i.e. returns something like "Example Resource"
```

## Instance Methods
The following methods are exposed on `ResourceData`.

| Method Name | Return Type | Lua API Version |
| --- | --- | --- |
| `readByte` | Integer | v0.0.1 |
| `readShort` | Integer | v0.0.1 |
| `readLong` | Integer | v0.0.1 |
| `readQuad` | Integer | v0.0.1 |
| `readSignedByte` | Integer | v0.0.1 |
| `readSignedShort` | Integer | v0.0.1 |
| `readSignedLong` | Integer | v0.0.1 |
| `readSignedQuad` | Integer | v0.0.1 |
| `readPStr` | String | v0.0.1 |
| `readCStr` | String | v0.0.1 |
| `readCStrOfLength` | Integer | v0.0.1 |

---
### `ResourceData:readByte`, `ResourceData:readSignedByte`
Read a single byte (unsigned/signed) from the current position in the `ResourceData` and returns an Integer value representing it.

##### Example
```lua
local type = res:readByte()
local type = res:readSignedByte()
```


---
### `ResourceData:readShort`, `ResourceData:readSignedShort`
Read a single short value (unsigned/signed, 2 bytes) from the current position in the `ResourceData` and returns an Integer value representing it.

_Note: This will automatically handle endianess of the value for the host platform_

##### Example
```lua
local length = res:readShort()
local length = res:readSignedShort()
```

---
### `ResourceData:readLong`, `ResourceData:readSignedLong`
Read a single long value (unsigned/signed, 4 bytes) from the current position in the `ResourceData` and returns an Integer value representing it.

_Note: This will automatically handle endianess of the value for the host platform_

##### Example
```lua
local cost = res:readLong()
local cost = res:readSignedLong()
```

---
### `ResourceData:readQuad`, `ResourceData:readSignedQuad`
Read a single quad value (unsigned/signed, 8 bytes) from the current position in the `ResourceData` and returns an Integer value representing it.

_Note: This will automatically handle endianess of the value for the host platform_

##### Example
```lua
local contrib = res:readQuad()
local contrib = res:readSignedQuad()
```

---
### `ResourceData:readPStr`
Read a Pascal encoded string from the Resource Data. Pascal encoded strings have a simple string length encoded at the start of the string as a single unsigned byte that is equal to the number of bytes in the string. They are limited to 255 characters due to the use of a single byte encoding their length.

##### Example
```lua
local name = res:readPStr()
```


---
### `ResourceData:readCStr`
Read a C encoded string from the Resource Data. C encoded strings are NUL-Terminated and do not have a limit upon their length, but can and have been the source of many crashes and bugs.

_Warning: Due to the potential bugs that can be introduced through the use of unbounded C-Strings, you should prefer the use of `ResourceData:readCStrOfLength` where possible._

##### Example
```lua
local name = res:readCStr()
```


---
### `ResourceData:readCStrOfLength`
Read a string of the specified length from the `ResourceData`. The precise number of bytes will be read from the data, ensuring that overflow bugs can not occur. This requires that the data contain strings that either truncated, or padded out to the expected size. Kestrel will not reconcile differences between what is requested and what is present.

##### Example
```lua
local name = res:readCStrOfLength(32)
```
