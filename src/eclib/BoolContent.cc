/*
 * (C) Copyright 1996-2012 ECMWF.
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 * In applying this licence, ECMWF does not waive the privileges and immunities 
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */


#include "eclib/BoolContent.h"
#include "eclib/Translator.h"
#include "eclib/JSON.h"

ClassSpec BoolContent::classSpec_ = {&Content::classSpec(),"BoolContent",};
Reanimator<BoolContent> BoolContent::reanimator_;

BoolContent::BoolContent(bool l):
    value_(l)
{
}

BoolContent::BoolContent(Stream& s):
	Content(s)
{
    s >> value_;
}

void BoolContent::encode(Stream& s) const
{
	Content::encode(s);
    s << value_;
}

BoolContent::~BoolContent()
{
}

void BoolContent::print(ostream& s) const
{
    s << ( value_ ? "true" : "false" );
}

void BoolContent::json(JSON& s) const
{
    s << value_;
}

int BoolContent::compare(const Content& other) const
{
    return -other.compareBool(*this);
}

int BoolContent::compareBool(const BoolContent& other) const
{
    if(value_ - other.value_)
        return 0;
    if(!value_)
		return -1;

	return 1;
}

void BoolContent::value(bool& l) const
{ 
    l = value_;
}

void BoolContent::value(string& s) const
{ 
    s = value_ ? "true" : "false";
}

Content* BoolContent::add(const Content& other) const
{
    return other.addBool(*this);
}


Content* BoolContent::sub(const Content& other) const
{
    return other.subBool(*this);
}

Content* BoolContent::mul(const Content& other) const
{
    return other.mulBool(*this);
}

Content* BoolContent::div(const Content& other) const
{
    return other.divBool(*this);
}

Content* BoolContent::mod(const Content& other) const
{
    return other.modBool(*this);
}
