/*
 * (C) Copyright 1996-2012 ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

#include "eckit/log/Log.h"
#include "eckit/runtime/Tool.h"

#include "eckit/maths/IfElse.h"
#include "eckit/maths/Lambda.h"
#include "eckit/maths/ParamDef.h"
#include "eckit/maths/Call.h"

#include "eckit/maths/Math.h"

using namespace eckit;
using namespace eckit::maths;

using namespace eckit;

//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

class YCombinator : public Tool {
public:

    YCombinator(int argc,char **argv): Tool(argc,argv) {
    }

    ~YCombinator() {
    }

    virtual void run();
};

//-----------------------------------------------------------------------------

void YCombinator::run()
{

    Math Y = maths::call(
            maths::lambda("m",
            maths::call(maths::lambda("builder", "x",
                         maths::call(Math("builder"),Math("builder"), Math("x"))),
                         maths::lambda("recurse", "n",
                         maths::ifelse(Math("n") == Math(0.0), Math(1.0),
                                       Math(maths::call(Math("recurse"),Math("recurse"), Math("n") - Math(1.0))) * Math("n"))),
                                     Math("m"))));

    cout << "-----------------------" << endl;
    cout << Y << endl;
    cout << "-----------------------" << endl;
    cout << Y(5.0) << endl;

}

//-----------------------------------------------------------------------------

int main(int argc,char **argv)
{
    YCombinator app(argc,argv);
    app.start();
    return 0;
}
