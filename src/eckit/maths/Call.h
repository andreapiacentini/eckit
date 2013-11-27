/*
 * (C) Copyright 1996-2013 ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

/// @file Call.h
/// @author Tiago Quintino
/// @date November 2013

#ifndef eckit_maths_Call_h
#define eckit_maths_Call_h

#include "eckit/maths/Function.h"

namespace eckit {
namespace maths {

//--------------------------------------------------------------------------------------------

/// Generates a Call combination of vectors
class Call : public Function {

public: // methods

    static std::string className() { return "Call"; }

    Call( ExpPtr f );
    Call( ExpPtr f, ExpPtr a);
    Call( ExpPtr f, ExpPtr a, ExpPtr b);

    virtual std::string typeName() const { return Call::className(); }

    virtual std::string returnSignature() const;

    virtual ExpPtr evaluate( Scope& ctx ) const;
    virtual void asCode( std::ostream& o ) const;
    virtual ExpPtr cloneWith(args_t &a) const;

};

//--------------------------------------------------------------------------------------------

ExpPtr call( ExpPtr f);
ExpPtr call( ExpPtr f, ExpPtr a);
ExpPtr call( ExpPtr f, ExpPtr a, ExpPtr b);

//--------------------------------------------------------------------------------------------

} // namespace maths
} // namespace eckit

#endif
