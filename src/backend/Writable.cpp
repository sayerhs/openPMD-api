/* Copyright 2017-2021 Fabian Koller
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
#include "openPMD/backend/Writable.hpp"
#include "openPMD/IO/AbstractIOHandler.hpp"
#include "openPMD/Series.hpp"
#include "openPMD/auxiliary/DerefDynamicCast.hpp"


namespace openPMD
{
    Writable::Writable(Attributable* a)
            : abstractFilePosition{nullptr},
              IOHandler{nullptr},
              attributable{a},
              parent{nullptr},
              dirty{true},
              written{false}
    { }

    void
    Writable::seriesFlush()
    {
        Writable * findSeries = this;
        while( findSeries->parent )
        {
            findSeries = findSeries->parent;
        }
        Series & series =
                auxiliary::deref_dynamic_cast< Series >( findSeries->attributable );
        series.flush_impl(
                series.iterations.begin(),
                series.iterations.end()
                //, IOHandler->m_flushLevel
        );
    }

} // openPMD
