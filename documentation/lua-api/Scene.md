# Kestrel Lua Bridge
## Scene API
### Introduction

The `Scene` class is used encapsulate a particular given content mode of 
the engine. For example a scene might be used to deliver a series of 
images as a "slide show", the actual game play or a menu screen.

Scenes operate via a stack, with scenes that are pushed on to the stack
becoming the new scene. "_Popping_" from the current scene will restore
the previous one.

---
## Functions

#### Scene(name)

| Argument | Type | Purpose |
| --- | --- | --- |
| name | String | The name of the scene being loaded. Value is not important. |

Create a new scene with the given name. The scene is not pushed to the stack
upon creation.

---

#### Scene:attachScript(id)

| Argument | Type | Purpose |
| --- | --- | --- |
| id | Integer | The Resource ID of a Lua Script. |

Load the script with the specified ID and attach it to the scene. The script
will be played when the scene is presented. 

---

#### Scene:present()

Presents the scene to the user by pushing it to the top of the stack.
