# std::any learn how to use it

## Introduction
The goal of **std::any** is to replace the use **void***. You could still use void* but it is an unsafe pattern and like any raw pointer you have to manage the lifetime of your object.

## Basic usage

### create the variable

There are several ways you can create std::any object:

* a default initialisation - then the object is empty
* a direct initialisation with a value/object
* in place std::in_place_type
* via std::make_any

Both std::make_any and std::in_place_type efficently create in place object.

### change the variable

## access the stored Value
