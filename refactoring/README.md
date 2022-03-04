## refactoring with std::optional and std::variant

Following the [std::optional](https://github.com/gaelmoccand/Cpp-Daily/blob/develop/optional/README.md)
ob/develop/any/README.md) and [std::variant](https://github.com/gaelmoccand/Cpp-Daily/blob/develop/variant/README.md) subjects.
Let's have a look at some concrete examples.

### 1. refactoring with optional a legacy method

Let's have a look how to refactor a legacy method [1]:

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

Consider a function that takes the current mouse selection for a game. The function scans the selected range and computes several outputs[2]:
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

First improvment might be to use tuple to follow the C++ core F20 rules :
"return multiple output  values, prefer returning a tuple or struct"

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
It is better now because we do not need to check the raw pointers and code is more expressive.

We can use C++17 structure binding on the caller side :

```cpp
auto [ok, anyCivil, anyCombat, numAnim] = CheckSelectionVer2(sel);
if (ok)
{
    // no error
}
```

Still we might forget the order of the ouptut and might be also tricky to extend the function.


### 2.2 using a struct

Outputs param. are related data. So it is a good idea to wrap them together into a struct:

```cpp
struct SelectionData
{
    bool anyCivilUnits {false};
    bool anyCombatUnits {false};
    int numAnimating {0};
};

std::pair<bool, SelectionData> CheckSelectionVer3(const ObjSelection &objList)
{
    SelectionData out;

    if (!objList.IsValid())
        return {false, out};
    // scan...
    return {true, out};
}

// caller :
if (auto [ok, selData] = CheckSelectionVer3(sel); ok)
{
    // ...
}

```

std::pair is used because we want to keep the error flag and it is not part of the _struct_.
The advantage now is that if we want to add a new param. then we only need to exted the _struct_.

But Hey the pattern _std::pair<bool,CustomType>_  looks familiar. We might use _std::optional_ instead.


### 2.3 using std::optional

As mentioned earlier, sometime we want to achieve a so called "nullable" type. It either contains a value, or it's empty.
That seems to be a good alternative for this code. We can remove the ok variable and rely on the semantics of the optional.

```cpp
std::optional<SelectionData> CheckSelectionVer4(const ObjSelection &objList)
{
    if (!objList.IsValid())
        return std::nullopt;

    SelectionData out;
    // scan...
    return out;
}


// And the caller site:
if (auto ret = CheckSelectionVer4(sel); ret.has_value())
{
    // access via *ret or even ret->
    // ret->numAnimating
}
```

The advantages of using std::optional is a clean and expressive form


### 2.4 using std::variant

Using std::optional does not tell why the error is raised because we do not have any error code to show the reason.
If this information is required then you should probably use std::variant.

```cpp
enum class [[nodiscard]] ErrorCode
{
    InvalidSelection,
    Undefined
};

variant<SelectionData, ErrorCode> CheckSelectionVer5(const ObjSelection &objList)
{
    if (!objList.IsValid())
        return ErrorCode::InvalidSelection;

    SelectionData out;
    // scan...

    return out;
}

// on the caller side.
if (auto ret = CheckSelectionVer5(sel);std::holds_alternative<SelectionData>(ret))
{
    std::cout << "ok..." << std::get<SelectionData>(ret).numAnimating << '\n';
}
else
{
    switch (std::get<ErrorCode>(ret))
    {
        case ErrorCode::InvalidSelection:
            std::cerr << "Invalid Selection!\n";
            break;
        case ErrorCode::Undefined:
            std::cerr << "Undefined Error!\n";
            break;
    }
}

```

It is then possible to return error code and respond to it compare to the std::optional solution.

As it can be seen the code uses std::variant with two possible alternatives: either SelectionData or ErrorCode.
It's almost like a pair, except that you'll always see one active value.



## References
1. https://www.fluentcpp.com/2016/11/24/clearer-interfaces-with-optionalt
2. https://www.cppstories.com/2018/04/refactoring-with-c17-stdoptional/#the-code
3. https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#f20-for-out-output-values-prefer-return-values-to-output-parameters





