/**
 * @proj romp
 * @author Andrew Osterhout
 * @author Ajantha Varadharaaj
 * @org University of Utah (UofU) School of Computing (SoC)
 * @org Center for Parallel compute at Utah (CPU)
 * @org <a href="https://civic-fv.github.io">Civic-fv NSF Grant</a>
 * @org Ganesh Gopalakrishnan's Research Group
 * @file throw_nested_error.hpp
 * 
 * @brief util function for joining and throwing a nested rumur error, for easy nested error traces.
 * 
 * @date 2022/06/28
 * @version 0.1
 */

#pragma once

#include <rumur/except.h>
#include <string>
#include <memory>
#include <regex>

namespace rumur {

class NestedError : public Error {
public:
  typedef std::string format_str;
  typedef char* format_str_;
private:
  const std::unique_ptr<const NestedError> prev_error;
  static const format_str default_format;
  static const std::regex loc_re("\\{[Ll][Oo][Cc](?:[Aa][Tt][Ii][Oo][Nn])?\\}");
  static const std::regex msg_re("\\{[Mm][Ss][Gg]|[Mm][Ee][Ss][Ss][Aa][Gg][Ee]|[Ww][Hh][Aa][Tt]\\}");
  static const std::regex nl_re("\\n");
public:
  NestedError(const std::string msg_, const location loc_, const NestedError& er);
  NestedError(const char* msg_, const location loc_, const NestedError& er);
  NestedError(const std::string msg_, const location loc_);
  NestedError(const char* msg_, const location loc_);
  NestedError(const std::string msg_, const location loc_, const Error& er);
  NestedError(const char* msg_, const location loc_, const Error& er);
  char* what() const;
  char* what(const format_str& format) const;
  char* what(const format_str_ format) const;
private:
  void format_what(const std::string& prefix, std::vector<std::string>& msg, const format_str& format);
};

}
