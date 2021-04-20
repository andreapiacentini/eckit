/*
 * (C) Copyright 1996- ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

#include <cmath>
#include <iostream>
#include <string>

#include "eckit/eckit_config.h"
#include "eckit/io/URLHandle.h"

#include "eckit/testing/Test.h"

using namespace std;
using namespace eckit;
using namespace eckit::testing;

namespace eckit {
namespace test {

//----------------------------------------------------------------------------------------------------------------------

CASE("Get URL without conten-lenght") {
    PathName out("/tmp/test_urlhandle_get.html");
    {
        URLHandle h("https://www.ecmwf.int");
        h.saveInto(out);
    }
    Log::info() << out << " size " << out.size() << std::endl;
    EXPECT(out.size() > Length(0));
    out.unlink();
}

// TODO Not working. Downloads zero length file
//
CASE("test_urlhandle_redirect") {
    PathName out("/tmp/test_urlhandle_get.html");
    {
        URLHandle h("https://get.ecmwf.int/atlas/grids/orca/v0/ORCA2_T.atlas");
        h.saveInto(out);
    }
    Log::info() << out << " size " << out.size() << std::endl;
    EXPECT(out.size() > Length(0));
    out.unlink();
}

CASE("Get URL small file 41 bytes") {
    PathName out("/tmp/t.grib.md5");
    {
        URLHandle h("http://download.ecmwf.org/test-data/eckit/tests/io/t.grib.md5");
        h.saveInto(out);
    }
    Log::info() << out << " size " << out.size() << std::endl;
    EXPECT(out.size() == Length(41));
    out.unlink();
}

CASE("Get URL 800K file") {
    PathName out("/tmp/t.grib.md5");
    {
        URLHandle h("http://download.ecmwf.org/test-data/multio/tests/server/single-field.grib");
        h.saveInto(out);
    }
    Log::info() << out << " size " << out.size() << std::endl;
    EXPECT(out.size() == Length(822360));
    out.unlink();
}

CASE("Handle URLException 404") {
    PathName out("/tmp/foobar");
    URLHandle h("http://download.ecmwf.org/test-data/eckit/tests/io/foobar");
    try {
        h.saveInto(out);
    }
    catch (eckit::URLException& e) {
        EXPECT(e.code() == 404);
    }
}

//----------------------------------------------------------------------------------------------------------------------

}  // namespace test
}  // namespace eckit

int main(int argc, char** argv) {
    return run_tests(argc, argv);
}
