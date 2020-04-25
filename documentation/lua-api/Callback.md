# Kestrel Lua Bridge
## Callback API

---

### Introduction

The `Callback` class can be used to instruct the Kestrel engine the return 
path and/or callback destination for a given function.

For example:

```lua
delayedThing(Callback.namedFunction("myFunction"))
function myFunction()
    ` Do something here...
end
```

### Functions

---
#### Callback.namedFunction(name)

| Argument | Type | Purpose |
| --- | --- | --- |
| name | String | The name of the function to be called back to. |

Create a new call back referencing a function that has already been loaded
in the Lua VM.

---

#### Callback.script(id)

| Argument | Type | Purpose |
| --- | --- | --- |
| id | Integer | The Resource ID of a Lua Script. |

Create a new call back referencing a lua script resource that is currently
loaded into the Kestrel Engine. 

**Warning:** This is not recommended for use in repeated callbacks, and 
should only be used for one-shot events where a script will only be loaded
once per session.