/*
 * (C) Copyright 1996-2013 ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

/// @file Count.h
/// @author Tiago Quintino
/// @date November 2013

#ifndef eckit_xpr_Count_h
#define eckit_xpr_Count_h

#include "eckit/xpr/Function.h"
#include "eckit/xpr/Undef.h"


namespace eckit {
namespace xpr {

//--------------------------------------------------------------------------------------------

/// Generates a Count combination of vectors
class Count : public Function {

public: // methods

    static std::string className() { return "Count"; }

    Count( ExpPtr e = undef() );

private: // methods

    Count( args_t& a );

    virtual std::string typeName() const { return className(); }

    virtual std::string returnSignature() const;

    virtual ExpPtr evaluate( Scope& ctx ) const;

    virtual void asCode( std::ostream& ) const;

    virtual ExpPtr cloneWith(args_t& a) const;

    virtual ExpPtr optimise(size_t depth) const;

};

//--------------------------------------------------------------------------------------------

ExpPtr count( ExpPtr e );

//--------------------------------------------------------------------------------------------

} // namespace xpr
} // namespace eckit

#endif
