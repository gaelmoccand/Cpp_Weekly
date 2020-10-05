## rvalue reference and std::move

### 1. Definitions

 - an **Lvalue** denotes an object whose **resource cannot be reused**, which includes most objects that we can think of in code. Lvalues include expressions that designate objects directly by their names but not only.
 - an **Rvalue** denotes an object whose **resource can be reused**, that is to say a **disposable object**. This typically includes temporary objects as they can’t be manipulated at the place they are created and are soon to be destroyed.
 - an **Lvalue** reference is a reference that binds to an **Lvalue**
 - an **Rvalue** reference is a reference that binds to an **Rvalue**
 - there can be **Lvalue const** reference binding to an **Rvalue** [1],[2]


```cpp
    int x = 123; // literal "123" is an Rvalue that must be stored into a Lvalue (x)
    std::string s1 = "Hello ";
    std::string s2 = "World ";
    std::string s3 = s1 + s2; // create a temporary object result of + so an Rvalue that must be stored in a Lvalue (s3)
    int& x = 123;       // Error
    const int& x = 123; // OK: Lvalue const bound to Rvalue
```

### 2. Rvalue reference, Lvalue the Context matters

Sometimes **Rvalue reference can themselves be Lvalue** (see code snipet below) [1]
std::move does not move anything but it just casting an lvalue to an rvalue reference. 


```cpp
    void f(MyClass&& rhs)
    {
        // Within function f() context, rhs is an Lvalue since it has a Name.
        // In the context of f() rhs is not a temporary objet
    } 

    f(MyClass()); // In the caller context, rhs is a temporary object so it is a Rvalue reference

    MyClass lfs;
    f(lfs); // Error: Rvalue reference can't be bind to Lvalue

    f(std::move(lfs)); // OK: lfs is cast to Rvalue reference
```

### 3. RVO and object creation
Have a look at the main.cpp code and play with it. 
RVO basically means the compiler is allowed to avoid creating temporary objects for return values, even if they have side effects.[3]
Without RVO the compiler creates 3 Vector objects instead of only 1:
1. A temporary object inside factory() (using ctor)
2. A temporary object for the returned object inside main() (mv ctor);
3. The named object Vector (2nd move ctor).


```cpp

class Vector{
 private:
    double* elem;
    size_t sz;
 public:
    explicit Vector(size_t s);              // ctor
    ~Vector(){delete[] elem;}               // dtor

    Vector(const Vector& vec);              // cp ctor
    Vector& operator=(const Vector& vec);   // cp assignment operator

    Vector( Vector&& vec);                  // mv ctor
    Vector& operator=( Vector&& vec);       // cp assignement operator
};

Vector factory(size_t s)
{
    return Vector(s);   // RVO to avoid copying Vector local variable
}

int main()
{
    Vector vec = factory(4);
}
```
### 4. Apply std::move to rvalue reference being return from fct that return by value but not on local object

Assuming that Matrix type supports move construction, casting lhs to an rvalue in the return statement will then increase efficency 
by moving lhs instead of copying it. If Matrix type does not support move ctor yet , then it won't hurts

```cpp
Matrix 
operator+(Matrix&& lhs, const Matrix& rhs)
{
    lhs+=rhs;
    return std::move(lhs); // if mv ctor is supported then move instead of copy 
}
```

However you should **NOT try to std::move on local objects** because then the compiler will not be able to use RVO.
Have a look at this example:

```cpp
Widget makeWidget()
{
    Widget w;
    ...
    return std::move(w); // Don't do this ! RVO won't work an
}
```
Because of std::move, what is being return is not the local object but the reference to this object (this is what std::move does).
RVO does not occure in this case (RVO only performs if what's being returned is a local object not in this case a reference to w)

Never apply std::move if they would otherwise be eligible for RVO. [4]

```cpp
Widget makeWidget()
{
    Widget w; // local variable
    ...
    return w; // Compiler treat w as rvalue -> std::move(w)
}
```
Compiler must treat it as if it had been written this way:

```cpp
Widget makeWidget()
{
    Widget w;
    ...
    return std::move(w); 
}
```
Same apply for by-value function parameters. Not eligible for copy elision but compilers will treat them as rvalue

## References
1. https://www.fluentcpp.com/2018/02/06/understanding-lvalues-rvalues-and-their-references/
2. https://www.internalpointers.com/post/c-rvalue-references-and-move-semantics-beginners
3. https://shaharmike.com/cpp/rvo/
4. https://www.oreilly.com/library/view/effective-modern-c/9781491908419/ item 25


