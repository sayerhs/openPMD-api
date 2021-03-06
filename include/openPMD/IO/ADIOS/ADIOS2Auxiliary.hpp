/* Copyright 2017-2021 Franz Poeschel.
 *
 * This file is part of openPMD-api.
 *
 * openPMD-api is free software: you can redistribute it and/or modify
 * it under the terms of of either the GNU General Public License or
 * the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * openPMD-api is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License and the GNU Lesser General Public License
 * for more details.
 *
 * You should have received a copy of the GNU General Public License
 * and the GNU Lesser General Public License along with openPMD-api.
 * If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "openPMD/config.hpp"
#if openPMD_HAVE_ADIOS2
# include "openPMD/Dataset.hpp"
# include "openPMD/Datatype.hpp"

# include <adios2.h>

# include <complex>
# include <stdexcept>
# include <utility>
# include <vector>

namespace openPMD
{
namespace detail
{
    // ADIOS2 does not natively support boolean values
    // Since we need them for attributes,
    // we represent booleans as unsigned chars
    using bool_representation = unsigned char;

    template < typename T > struct ToDatatypeHelper
    {
        static std::string type( );
    };

    template < typename T > struct ToDatatypeHelper< std::vector< T > >
    {
        static std::string type( );
    };

    template < typename T, size_t n >
    struct ToDatatypeHelper< std::array< T, n > >
    {
        static std::string type( );
    };

    template <> struct ToDatatypeHelper< bool >
    {
        static std::string type( );
    };

    struct ToDatatype
    {
        template < typename T > std::string operator( )( );


        template < int n > std::string operator( )( );
    };

    /**
     * @brief Convert ADIOS2 datatype to openPMD type.
     * @param dt
     * @return
     */
    Datatype fromADIOS2Type( std::string const & dt );

    enum class VariableOrAttribute : unsigned char
    {
        Variable,
        Attribute
    };

    template < typename T > struct AttributeInfoHelper
    {
        static Extent
        getSize(
            adios2::IO &,
            std::string const & attributeName,
            VariableOrAttribute );
    };

    template < > struct AttributeInfoHelper< std::complex< long double > >
    {
        static Extent
        getSize( adios2::IO &, std::string const &, VariableOrAttribute )
        {
            throw std::runtime_error(
                "[ADIOS2] Internal error: no support for long double complex attribute types" );
        }
    };

    template < typename T > struct AttributeInfoHelper< std::vector< T > >
    {
        static Extent
        getSize(
            adios2::IO &,
            std::string const & attributeName,
            VariableOrAttribute );
    };

    template < > struct AttributeInfoHelper< std::vector< std::complex< long double > > >
    {
        static Extent
        getSize( adios2::IO &, std::string const &, VariableOrAttribute )
        {
            throw std::runtime_error(
                "[ADIOS2] Internal error: no support for long double complex vector attribute types" );
        }
    };

    template < typename T, std::size_t n >
    struct AttributeInfoHelper< std::array< T, n > >
    {
        static Extent
        getSize(
            adios2::IO & IO,
            std::string const & attributeName,
            VariableOrAttribute voa )
        {
            return AttributeInfoHelper< T >::getSize( IO, attributeName, voa );
        }
    };

    template <> struct AttributeInfoHelper< bool >
    {
        static Extent
        getSize(
            adios2::IO &,
            std::string const & attributeName,
            VariableOrAttribute );
    };

    struct AttributeInfo
    {
        template< typename T, typename... Params >
        Extent
        operator()( Params &&... );

        template < int n, typename... Params >
        Extent operator( )( Params &&... );
    };

    /**
     * @brief Get openPMD datatype of attribute within given ADIOS IO.
     *
     * @param IO The IO within which to retrieve the attribute.
     * @param attributeName The full ADIOS name of the attribute.
     * @param verbose If true, print a warning if not finding the attribute.
     * @return The openPMD datatype corresponding to the type of the attribute.
     *         UNDEFINED if attribute is not found.
     */
    Datatype
    attributeInfo(
        adios2::IO & IO,
        std::string const & attributeName,
        bool verbose,
        VariableOrAttribute = VariableOrAttribute::Attribute );

    template< typename T >
    struct IsTrivialType
    {
        constexpr static bool val = true;
    };

    template< typename T >
    struct IsTrivialType< std::vector< T > >
    {
        constexpr static bool val = false;
    };

    template< typename T, size_t n >
    struct IsTrivialType< std::array< T, n > >
    {
        constexpr static bool val = false;
    };

    template<>
    struct IsTrivialType< bool >
    {
        constexpr static bool val = false;
    };

    template<>
    struct IsTrivialType< std::complex< long double > >
    {
        constexpr static bool val = false;
    };

    template<>
    struct IsTrivialType< std::vector< std::complex< long double > > >
    {
        constexpr static bool val = false;
    };
} // namespace detail

} // namespace openPMD

#endif // openPMD_HAVE_ADIOS2
