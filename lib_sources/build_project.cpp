
#include <build_project.hpp>

namespace build_project {

/// Only used for initialisation of repository
auto get_greeting() noexcept -> string_view {
  return "Hello, world...";
}

} // namespace build_project
