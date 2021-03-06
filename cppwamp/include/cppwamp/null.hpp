/*------------------------------------------------------------------------------
                Copyright Butterfly Energy Systems 2014-2015.
           Distributed under the Boost Software License, Version 1.0.
              (See accompanying file LICENSE_1_0.txt or copy at
                    http://www.boost.org/LICENSE_1_0.txt)
------------------------------------------------------------------------------*/

#ifndef CPPWAMP_NULL_HPP
#define CPPWAMP_NULL_HPP

//------------------------------------------------------------------------------
/** @file
    Contains the declaration of Null, one of the bound types used by Variant. */
//------------------------------------------------------------------------------

#include <ostream>

namespace wamp
{

//------------------------------------------------------------------------------
/** Type used to represent a null or empty Variant value.
    The `wamp::null` constant should be used to nullify a `Variant`, and
    for checking if a `Variant` is empty:
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    using namespace wamp;
    Variant v(42); // v contains an integer
    v = null; // v is now empty

    // Check if v is null
    if (v == null)
        std::cout << "v is empty\n";
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    @see wamp::null */
//------------------------------------------------------------------------------
struct Null
{
    /// Default constructor. For instantiating const object.
    constexpr Null() {}

    /// Compares two `Null` objects for equality. @return always `true`.
    bool operator==(Null) const;

    /// Compares two `Null` objects for inequality. @return always `false`.
    bool operator!=(Null) const;

    /// Performs less-than comparison on two `Null` objects.
    /// @return always `false`.
    bool operator<(Null)  const;
};

//------------------------------------------------------------------------------
/** @relates Null
    Outputs a `Null` object to the given output stream.
    The string `"null"` (without the quotes) will be outputted. */
//------------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& out, Null);

//------------------------------------------------------------------------------
/** Constant Null object that can be assigned to, or compared with a Variant. */
//------------------------------------------------------------------------------
constexpr Null null;


} // namespace wamp

#include "internal/null.ipp"

#endif // CPPWAMP_NULL_HPP
