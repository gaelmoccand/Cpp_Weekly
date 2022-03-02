## refactoring woth std::optional and std::variant

Following the [std::optional](https://github.com/gaelmoccand/Cpp-Daily/blob/develop/optional/README.md)
ob/develop/any/README.md) and [std::variant](https://github.com/gaelmoccand/Cpp-Daily/blob/develop/variant/README.md) subjects,
let's have a look at some concrete examples.

### 1. refactoring with optional a legacy method

Let's have a look how to refactor a legacy method:

```cpp
    int oldFunc(); // when -1 is return it means no value
```

We would rewrite it so that it benefits from optional to express the **no-value**, like this:

```cpp
    std::optional<int> newFunc();
```

And to limit the amount of code modified at the same time we can still keep the old function, and make it call the new one:

```cpp
int oldF()
{
    return newF().get_value_or(-1);
}
```

### 2. refactoring with optional game exmple

Consider a function that takes the current mouse selection for a game. The function scans the selected range and computes several outputs:
* the number of animating objects
* if there are any civil units in the selection
* if there are any combat units in the selection

```cpp
class ObjSelection
{
public:
    bool IsValid() const {return true;}
    // more code...
};

bool CheckSelectionVer1(const ObjSelection &objList,
                        bool *pOutAnyCivilUnits,
                        bool *pOutAnyCombatUnits,
                        int *pOutNumAnimating);
```

The drawback with this code is it has many output param using ugly raw pointer  and return a boolean to indicate success or failure.

Imrpovements could be :

* Output parameters: Core guidelines suggests not to use them.
* using raw pointers you have to check if they are valid. You might get away with the checks if you use references for the output parameters.
* Might be tricky to extend the function with another param.

### 2.1 tuple version

```cpp
std::tuple<bool, bool, bool, int>
CheckSelectionVer2(const ObjSelection &objList)
{
    if (!objList.IsValid())
        return {false, false, false, 0};

    // local variables:
    int numCivilUnits = 0;
    int numCombat = 0;
    int numAnimating = 0;

    // scan...

    return {true, numCivilUnits > 0, numCombat > 0, numAnimating};
}
```
it is better now because we do not need to check the raw pointerand code is more expressive.

We can use C++17 sturcure binding on the caller side :

```cpp
auto [ok, anyCivil, anyCombat, numAnim] = CheckSelectionVer2(sel);
if (ok)
{
    // no error
}
```

Still we might forget the order of the ouptut and might be also tricky to extend the function.


## References
1. https://www.fluentcpp.com/2016/11/24/clearer-interfaces-with-optionalt




