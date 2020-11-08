#ifndef CPP_BUILD_PROJECT
#define CPP_BUILD_PROJECT

#include <string_view>

namespace build_project {

using std::string_view;

/// Only used for initialisation of repository
auto get_greeting() noexcept -> string_view;

} // namespace build_project

#endif