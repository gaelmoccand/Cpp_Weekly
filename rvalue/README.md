## rvalue reference , std::move and return value optimization

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

Sometimes **Rvalue reference can themselves be Lvalue** (see code snipet below) [1].

Please note that **std::move** does not move anything but it just casting an Lvalue to an Rvalue reference. 


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

RVO basically means the compiler is allowed to avoid creating temporary objects for return values, even if they have side effects.[3]
Without RVO the compiler creates 3 Vector objects instead of only 1 (check belwow the piece of code):
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
    return std::move(w); // Don't do this ! RVO won't work !
}
```
Because of std::move, what is being return is not the local object but the reference to this object (this is what std::move does),
RVO does not occur in this case (RVO only performs if what's being returned is a local object not in this case a reference to the object)

Never apply std::move if they would otherwise be eligible for RVO. [4]

```cpp
Vector makeVector()
{
    Vector vec{2}; // local variable
    ...
    return vec; // Compiler treat vec as rvalue -> std::move(w)
}

```
Compiler must either elide the copy of vec OR they must treat as if it had been written this way:

```cpp
Vector makeVector()
{
    Vector vec{2}; // local variable
    ...
    return std::move(vec); // treat vec as Rvalue because no copy elision was performed
}
```
Same apply for by-value function parameters. 
They are not eligible for copy elision but compilers will treat them as Rvalue:

```cpp
Vector makeVector(Vector vec) // by-value parameter of same type as fct return
{
    ...
    return std::move(vec); // treat vec as Rvalue because no copy elision was performed
}
```

### 5. Check that types you are using have cheap move operation or no move operation

Standard container contents are stored on the heap and a pointer points to the heap. 
It makes move operation really efficient O(1). Move operation just copy the pointer to the conainer's contents from the source 
container to the targer and set the source's pointer to null.

std::array is differant from other std container. std::array has not such ponter because the data contents are stored 
directly in the std::array object.
So for std:array move operation must copy or moved each element in the container O(n).
So move for std::array is not cheap.

Note also that small string optimization (SSO, <= 15 char), stores small strings in a buffer within the string object.
There is also no heap allocation in this SSO case.

### 6. Avoid overloading on universal reference

See effective modern c++ Item26 [7]

### 7. Copying from an lvalue, moving from an rvalue in a ctor 

The goal here is to avoid copy in the ctor. A copy occurs when passing Lvalue to ctor
but only move is done when a Rvalue is pass.

```cpp
template<class T>
class TextDisplayer
{
    public:
        explicit TextBox(const std::string& text) : m_text(text) {} // 1 copy
        explicit TextBox(std::string&& text) : m_text(std::move(text)) {} // no copy only mv
    private:
    T m_text;
};
```

The solution for C++ 17 is to use so called deduction guide See fluentcpp.com [6] 

```cpp
template<class T>
class TextDisplayer
{
    public:
        explicit TextDisplayer(T&& text) : m_text(std::forward<T>(text)) {}
    private:
    T m_text;
};
template<class T> TextDisplayer(T&&) -> TextDisplayer<T>; // deduction guide
...

std::string txt = "Hello World";
TextDisplayer displayer1(txt); // ok
TextDisplayer displayer2(get_string_from_file()); // error if deduction guide is missing 
TextDisplayer displayer1("Hello World");    // error if deeduction guide is missing
```



  

## References
1. https://www.fluentcpp.com/2018/02/06/understanding-lvalues-rvalues-and-their-references/
2. https://www.internalpointers.com/post/c-rvalue-references-and-move-semantics-beginners
3. https://shaharmike.com/cpp/rvo/
4. https://www.oreilly.com/library/view/effective-modern-c/9781491908419/item25
5. https://www.oreilly.com/library/view/effective-modern-c/9781491908419/item27
6. https://www.fluentcpp.com/2018/07/17/how-to-construct-c-objects-without-making-copies/
7. https://www.oreilly.com/library/view/effective-modern-c/9781491908419/item26


