/*
 * POOSV EPFL 2015-2022
 * Marco Antognini
 */

#ifndef INFOSV_JSONSERIALISER_HPP
#define INFOSV_JSONSERIALISER_HPP

#include <JSON/JSON.hpp>

#include <stdexcept>
#include <string>

namespace j
{

class BadPayload : public std::runtime_error
{
public:
    BadPayload(std::string const& msg);
};

class NoSuchFile : public std::runtime_error
{
public:
    NoSuchFile(std::string const& msg);
};

// Read a JSON value from a payload string
// Throw a BadPayload when the format is invalid
Value readFromString(std::string const& payload);

// Like `readFromString` but the payload is read from the given file
// Throw a BadPayload when the file's content has an invalid format
// Throw a NoSuchFile when the file doesn't exist
Value readFromFile(std::string const& filepath);

// Convert a JSON value to the corresponding payload string
std::string writeToString(Value const& value);

// Like `writeToString` but write it to the given file
void writeToFile(Value const& value, std::string const& fileapath);

} // j

#endif // INFOSV_JSONSERIALISER_HPP
