#ifndef CPP_BUILD_PROJECT
#define CPP_BUILD_PROJECT

#include <filesystem>
#include <initializer_list>
#include <memory>
#include <stdexcept>
#include <string_view>

// implementation-defined: relates to implementer of the functions declared here
// compiler-defined: relates to implementer of derived classes of `compiler_interface`
// platform-defined: should be a consensus over tools so it depends only on the targeted platform (i.e. file extensions .so/.dll)

namespace build_project {

// Declaration of classes defined in this header.
class preprocessor_args;
class flags;
class compiler_interface;

// Common using declarations
using std::initializer_list;
using std::move;
using std::size_t;
using std::string_view;
using std::unique_ptr;
using std::filesystem::path;

struct error_not_supported : std::runtime_error {
  using std::runtime_error::runtime_error; // inheriting constructors
};

struct tools {
  unique_ptr<compiler_interface> native; //< For native platform, i.e. code generators
  unique_ptr<compiler_interface> host;   //< For final-user platform, i.e. the actual program
  // TODO: Consider adding "target"? maybe useless?
  // TODO: Add dependency manager?
};

/// Parse the command-line arguments from `main(argc,argv)` and provide required tools for the asked build.
/// May call `exit()` if needed (i.e. after printing help or if detecting invalid argument).
/// Post-condition: pointers returned in `tools` are not null.
auto parse_args_get_tools(int argc, char const* const* argv, string_view info_message = "") -> tools;

enum class language {
  c89,
  c99,
  c11,
  c17,
  c_latest,
  cpp98,
  cpp03,
  cpp11,
  cpp14,
  cpp17,
  cpp20,
  cpp_latest
};

class project_file {
public:
  enum class kind {
    source,
    archive,
    dynlib,
    program,
    only_interface
  };
  // Implicit conversion "path -> input(path, kind::source)"
  project_file(path path, kind kind = kind::source) : _kind(kind), _path(move(path)) {}

  // Implicit taking of any path-like type (string_view, char const*, etc)
  // needed for inputting string literals in initializer_list<project_file>
  template <typename PathConvertible,
            typename = std::enable_if_t< // TODO: replace SFINAE with C++20 concepts
                std::is_convertible_v<PathConvertible, path>>>
  project_file(PathConvertible&& p, kind kind = kind::source)
      : project_file(path{std::forward<PathConvertible>(p)}, kind) {}

  auto get_path() const noexcept -> path const& { return _path; }
  auto get_kind() const noexcept -> kind { return _kind; }

private:
  kind _kind;
  path _path;
};

/// Helper functions for better syntax.
inline auto archive(path p) -> project_file { return project_file(move(p), project_file::kind::archive); }
inline auto dynlib(path p) -> project_file { return project_file(move(p), project_file::kind::dynlib); }
inline auto program(path p) -> project_file { return project_file(move(p), project_file::kind::program); }
inline auto only_interface(path p) -> project_file { return project_file(move(p), project_file::kind::only_interface); }

class compiler_interface {
public:
  // TODO: Specify how is formatted the identifier (normalized target triplet?)
  virtual auto platform_identifier() const noexcept -> string_view = 0;

  /// Main function to build the project.
  /// `dst` must have been generated with archive(), dynlib() or program().
  /// `[ inputs_data ; inputs_data + inputs_size )` can contain:
  /// - source and archive files if `dst` is an archive
  /// - source, archive and dynlib files if `dst` is a dynlib or a program
  /// All paths must be relative and inside the current directory.
  // TODO: Maybe virtual so it may be overridden?
  void create(project_file const& dst,
              project_file const* inputs_data, size_t inputs_size, // TODO: Use C++20 std::span or equivalent
              language lang = language::cpp_latest,
              preprocessor_args const* pp_args = nullptr,
              flags const* flags = nullptr);

  void create(project_file const& dst,
              initializer_list<project_file> inputs,
              language lang = language::cpp_latest,
              preprocessor_args const* pp_args = nullptr,
              flags const* flags = nullptr) {
    create(dst, inputs.begin(), inputs.size(), lang, pp_args, flags);
  }

  // TODO: Virtual constructors for preprocessor_args and flags

  /// Platform-defined renaming of filenames depending on file kind.
  /// For instance: `get_actual_filepath(dynlib("path/to/myproject")) -> "path/to/libmyproject.so"
  virtual auto get_actual_filepath(project_file const& f) const -> path = 0;

  virtual ~compiler_interface() noexcept = default;

private:
};

} // namespace build_project

#endif