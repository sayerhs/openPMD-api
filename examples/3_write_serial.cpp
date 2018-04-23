/* Copyright 2017-2018 Fabian Koller, Axel Huebl
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
#include <openPMD/openPMD.hpp>

#include <iostream>
#include <memory>
#include <numeric>


using std::cout;
using namespace openPMD;

int main(int argc, char *argv[])
{
    // user input: size of matrix to write, default 3x3
    size_t size = (argc == 2 ? atoi(argv[1]) : 3);

    // matrix dataset to write with values 0...size*size-1
    std::shared_ptr< double > global_data{
        new double[size*size],
        std::default_delete< double[] >()
    };
    std::iota(global_data.get(), global_data.get() + size*size, 0.);

    cout << "Set up a 2D square array (" << size << 'x' << size
         << ") that will be written\n";

    // open file for writing
    Series series = Series::create(
        "../samples/3_write_serial.h5",
        AccessType::CREATE
    );
    cout << "Created an empty " << series.iterationEncoding() << " Series\n";

    MeshRecordComponent &E =
      series
          .iterations[1]
          .meshes["E"][MeshRecordComponent::SCALAR];

    Datatype datatype = determineDatatype(global_data);
    Extent extent = {size, size};
    Dataset dataset = Dataset(datatype, extent);

    cout << "Created a Dataset of size " << dataset.extent[0] << 'x' << dataset.extent[1]
         << " and Datatype " << dataset.dtype << '\n';

    E.resetDataset(dataset);
    cout << "Set the dataset properties for the scalar field E in iteration 1\n";

    series.flush();
    cout << "File structure has been written\n";

    Offset offset = {0, 0};
    E.storeChunk(offset, extent, global_data);
    cout << "Stored the whole Dataset contents as a single chunk, "
            "ready to write content\n";

    series.flush();
    cout << "Dataset content has been fully written\n";

    return 0;
}