#include <memory>
#include <vector>

namespace clanguml {
namespace t00028 {

/// \clanguml{note[top] A class note.}
class A {
};

/// \clanguml{note[] B class note.}
class B {
};

///
/// @clanguml{note:t00028_class[bottom] C class note.}
/// This is class C.
class C {
};

/// \clanguml{note
/// D
/// class
/// note.}
class D {
};

/// \clanguml{note E template class note.}
template <typename T> class E {
    T param;
};

/// \clanguml{note:other_diagram[left] G class note.}
class G {
};

/// @clanguml{note[ bottom ] F enum note.}
enum class F { one, two, three };

/// \clanguml{note[right] R class note.}
class R {
    A aaa;

    B *bbb;

    C &ccc;

    std::vector<std::shared_ptr<D>> ddd;

    E<int> eee;

    G **ggg;
};

} // namespace t00028
} // namespace clanguml
