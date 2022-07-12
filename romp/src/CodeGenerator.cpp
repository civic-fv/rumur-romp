#include "CodeGenerator.hpp"
#include <cassert>
#include <cstddef>
#include <string>
#include <vector>


namespace romp {

const std::unordered_set<std::string> CodeGenerator::RESERVED_NAMES{ROMP_RESERVED_NAMES, ROMP_RESERVED_NAMES};

std::string CodeGenerator::M_FUNCTION__FUNC_ATTRS = "";

std::string CodeGenerator::M_RULE_GUARD__FUNC_ATTRS = "";
std::string CodeGenerator::M_RULE_ACTION__FUNC_ATTRS = "";

std::string CodeGenerator::M_STARTSTATE__FUNC_ATTRS = "";

std::string CodeGenerator::M_PROPERTY__FUNC_ATTRS = "";


std::string CodeGenerator::indentation() const {
  return std::string(indent_level * 2, ' ');
}

void CodeGenerator::indent() { indent_level++; }

void CodeGenerator::dedent() {
  assert(indent_level > 0 && "attempted negative indentation");
  indent_level--;
}

CodeGenerator::~CodeGenerator() {}

}
