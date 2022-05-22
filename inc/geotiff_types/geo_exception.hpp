#ifndef __GEOTIFF_TYPES_GEO_EXCEPTION_HPP__
#define __GEOTIFF_TYPES_GEO_EXCEPTION_HPP__

#include <string>
#include <exception>

class GeoException : public std::exception
{
public:
    GeoException(const std::string& msg = "", const int ec = 0);
    ~GeoException();

    const char* what() const noexcept;
    const int get_code() const;
    const std::string get_message() const;

private:
    const int m_error_code;
    const std::string m_message;
};

#endif // __GEOTIFF_TYPES_GEO_EXCEPTION_HPP__
