#include "CodeGenerator.h"
#include <cassert>
#include <cstddef>
#include <string>
#include <vector>


namespace romp {

std::string CodeGenerator::indentation() const {
  return std::string(indent_level * 2, ' ');
}

void CodeGenerator::indent() { indent_level++; }

void CodeGenerator::dedent() {
  assert(indent_level > 0 && "attempted negative indentation");
  indent_level--;
}

// get a white space string for the current indentation level
size_t CodeGenerator::get_scope_level() const {
  return scope_level;
}

// increase the current scope level
size_t CodeGenerator::enter_scope() {
  return (++scope_level);
}

// decrease the current scope level
size_t CodeGenerator::leave_scope() {
  return (--scope_level);
}

CodeGenerator::~CodeGenerator() {}

}
