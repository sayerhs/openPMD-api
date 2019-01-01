/* Copyright 2017-2019 Fabian Koller
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

#include <algorithm>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>
#include <cassert>


namespace openPMD
{
namespace auxiliary
{
inline bool
contains(std::string const &s,
         std::string const &infix)
{
    return s.find(infix) != std::string::npos;
}

inline bool
contains(std::string const &s,
         char const infix)
{
    return s.find(infix) != std::string::npos;
}

inline bool
starts_with(std::string const &s,
            std::string const &prefix)
{
    return (s.size() >= prefix.size()) &&
           (0 == s.compare(0, prefix.size(), prefix));
}

inline bool
starts_with(std::string const &s,
            char const prefix)
{
    return !s.empty() &&
           s[0] == prefix;
}

inline bool
ends_with(std::string const &s,
          std::string const &suffix)
{
    return (s.size() >= suffix.size()) &&
           (0 == s.compare(s.size() - suffix.size(), suffix.size(), suffix));
}

inline bool
ends_with(std::string const &s,
          char const suffix)
{
    return !s.empty() && s.back() == suffix;
}

inline std::string
replace_first(std::string s,
              std::string const& target,
              std::string const& replacement)
{
    std::string::size_type pos = s.find(target);
    if( pos == std::string::npos )
        return s;
    s.replace(pos, target.size(), replacement);
    s.shrink_to_fit();

    return s;
}

inline std::string
replace_last(std::string s,
             std::string const& target,
             std::string const& replacement)
{
    std::string::size_type pos = s.rfind(target);
    if( pos == std::string::npos )
        return s;
    s.replace(pos, target.size(), replacement);
    s.shrink_to_fit();

    return s;
}

inline std::string
replace_all(std::string s,
            std::string const& target,
            std::string const& replacement)
{
    std::string::size_type pos = 0;
    auto tsize = target.size();
    assert(tsize > 0);
    auto rsize = replacement.size();
    while (true)
    {
        pos = s.find(target, pos);
        if (pos == std::string::npos)
            break;
        s.replace(pos, tsize, replacement);
        // Allow replacing recursively, but only if
        // the next replaced substring overlaps with
        // some parts of the original word.
        // This avoids loops.
        pos += rsize - std::min(tsize - 1, rsize);
    }
    s.shrink_to_fit();
    return s;
}

inline std::vector< std::string >
split(std::string const &s,
      std::string const &delimiter,
      bool includeDelimiter = false)
{
    std::vector< std::string > ret;
    std::string::size_type pos, lastPos = 0, length = s.size();
    while( lastPos < length + 1 )
    {
        pos = s.find_first_of(delimiter, lastPos);
        if( pos == std::string::npos )
        {
            pos = length;
            includeDelimiter = false;
        }

        if( pos != lastPos )
            ret.push_back(s.substr(lastPos, pos + (includeDelimiter ? delimiter.size() : 0) - lastPos));

        lastPos = pos + 1;
    }

    return ret;
}

inline std::string
strip(std::string s, std::vector< char > to_remove)
{
    for( auto const& c : to_remove )
        s.erase(std::remove(s.begin(), s.end(), c), s.end());
    s.shrink_to_fit();

    return s;
}

inline std::string
join(std::vector< std::string > const& vs, std::string const& delimiter)
{
    switch( vs.size() )
    {
        case 0:
            return "";
        case 1:
            return vs[0];
        default:
            std::ostringstream ss;
            std::copy(vs.begin(),
                      vs.end() - 1,
                      std::ostream_iterator< std::string >(ss, delimiter.c_str()));
            ss << *(vs.end() - 1);
            return ss.str();
    }
}
} // auxiliary
} // openPMD
