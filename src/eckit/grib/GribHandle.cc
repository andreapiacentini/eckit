/*
 * (C) Copyright 1996-2013 ECMWF.
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 * In applying this licence, ECMWF does not waive the privileges and immunities 
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

#include <grib_api.h>

#include "eckit/exception/Exceptions.h"
#include "eckit/parser/StringTools.h"
#include "eckit/io/DataHandle.h"
#include "eckit/geometry/Point2.h"

#include "eckit/grib/GribHandle.h"
#include "eckit/grib/GribAccessor.h"

using namespace std;

namespace eckit {
namespace grib {

//------------------------------------------------------------------------------------------------------

GribHandle::GribHandle(grib_handle* handle) :
	handle_(handle),
	owns_handle_(true)
{
	ASSERT(handle_);
}

GribHandle::GribHandle(grib_handle& handle) :
	handle_(&handle),
	owns_handle_(false)
{
}

GribHandle::GribHandle(const Buffer& buffer, bool copy)
	: handle_(0),
	  owns_handle_(true)
{
	const char *message = buffer;
	ASSERT(strncmp(message,"GRIB", 4) == 0);

	grib_handle *h = 0;

	if(copy)
	{
		h = grib_handle_new_from_message_copy(0,const_cast<char*>(message),buffer.size());
	}
	else
	{
		h = grib_handle_new_from_message(0,const_cast<char*>(message),buffer.size());
	}

	handle_ = h;

	ASSERT(handle_);
}

GribHandle::~GribHandle()
{
	if( owns_handle_ && handle_ )
		grib_handle_delete(handle_);
}

std::string GribHandle::gridType() const
{
	return GribAccessor<std::string>("gridType")(handle_);
}

std::string GribHandle::geographyHash() const
{
	return grib_geography_hash(handle_);
}

long GribHandle::edition() const
{
	GribAccessor<long>("edition")(handle_);
}

size_t GribHandle::getDataValuesSize() const
{
    size_t count = 0;
    GRIB_CALL(grib_get_size(handle_, "values", &count));
    return count;
}

void GribHandle::getDataValues(double* values, const size_t& count) const
{
    ASSERT(values);
    size_t n = count;
    GRIB_CALL(grib_get_double_array(handle_,"values",values,&n));
    ASSERT(n == count);
}

double* GribHandle::getDataValues(size_t& count) const
{
    count = getDataValuesSize();

    double* values = new double[count];
    getDataValues(values,count);
    return values;
}

void GribHandle::setDataValues(const double *values, size_t count)
{
    ASSERT(values);
    GRIB_CALL(grib_set_double_array(handle_,"values",values,count));
}

void GribHandle::write( DataHandle& handle )
{
    const void* message;
    size_t length;
    GRIB_CALL(grib_get_message(handle_, &message, &length));
    ASSERT(handle.write(message, length) == length);
}

size_t GribHandle::write( Buffer& buff )
{
    size_t len = buff.size();
    GRIB_CALL( grib_get_message_copy( handle_, buff, &len )); // will issue error if buffer too small
	return len;
}

double GribHandle::latitudeOfFirstGridPointInDegrees() const
{
	return GribAccessor<double>("latitudeOfFirstGridPointInDegrees")(handle_);
}

double GribHandle::longitudeOfFirstGridPointInDegrees() const
{
	return GribAccessor<double>("longitudeOfFirstGridPointInDegrees")(handle_);
}

double GribHandle::latitudeOfLastGridPointInDegrees() const
{
	return GribAccessor<double>("latitudeOfLastGridPointInDegrees")(handle_);
}

double GribHandle::longitudeOfLastGridPointInDegrees() const
{
	return GribAccessor<double>("longitudeOfLastGridPointInDegrees")(handle_);
}

GribHandle* GribHandle::clone() const
{
    grib_handle* h = grib_handle_clone(handle_);
    if(!h)
        throw eckit::WriteError( std::string("failed to clone output grib") );

    return new GribHandle(h);
}

size_t GribHandle::nbDataPoints() const
{
	return GribAccessor<long>("numberOfDataPoints")(handle_);
}

//------------------------------------------------------------------------------------------------------

int grib_call(int code, const char *msg)
{
    if(code)
    {
        StrStream os;
        os << msg << ": " << grib_get_error_message(code) << StrStream::ends;
        throw Exception(std::string(os));
    }
	return code;
}

void read_latlon_from_grib(GribHandle& h, std::vector<geometry::LLPoint2>& points)
{
   size_t nb_nodes = h.nbDataPoints();

   points.resize(nb_nodes);

   /// It should be noted that grib iterator is *only* available for certain grids
   /// i.e for Spherical Harmonics it is not implemented.
   int err = 0;
   grib_iterator *i = grib_iterator_new( h.raw(), 0, &err );
   if ( err != 0 )
   {
	  throw SeriousBug(string("Error reading grib. Could not create grib_iterator_new"),Here()) ;
   }

   double lat   = 0.;
   double lon   = 0.;
   double value = 0.;

   size_t idx = 0;
   while( grib_iterator_next(i,&lat,&lon,&value) )
   {
	  points[idx].assign(lat,lon);
	  ++idx;
   }

   ASSERT( idx == nb_nodes );

   if ( grib_iterator_delete(i) != 0 )
	  throw SeriousBug(string("Error reading grib. Could not delete grib iterator"),Here()) ;
}

//------------------------------------------------------------------------------------------------------

} // namespace grib
} // namespace eckit
