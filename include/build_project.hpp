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
  cpp98,
  cpp03,
  cpp11,
  cpp14,
  cpp17,
  c_latest = c17,
  cpp_latest = cpp17
};

class compiler_interface {
public:
  // TODO: Specify how is formatted the identifier (normalized target triplet?)
  virtual auto platform_identifier() const noexcept -> string_view = 0;

  /// Platform-defined renaming of filenames depending on the file type
  /// For instance `dynlib("path/to/myproject") -> "path/to/libmyproject.so"`
  virtual auto archive(path p) const noexcept -> path = 0;
  virtual auto dynlib(path p) const noexcept -> path = 0;
  virtual auto program(path p) const noexcept -> path = 0;

  /// Main function to build the project.
  /// `dst` must have been generated with archive(), dynlib() or program().
  /// `[ inputs_data ; inputs_data + inputs_size )` can contain:
  /// - source and archive files if `dst` is an archive
  /// - source, archive and dynlib files if `dst` is a dynlib or a program
  /// All paths must be relative and inside the current directory.
  // TODO: Maybe virtual so it may be overridden?
  // TODO: Return paths to module interface units?
  // TODO: Should module interface units be compiled to objects and inserted inside archive and dynlib?
  void create(path dst,
              path const* inputs_data, size_t inputs_size, // TODO: Use C++20 std::span or equivalent
              language lang = language::cpp_latest,
              preprocessor_args const* pp_args = nullptr,
              flags const* flags = nullptr);

  void create(path dst,
              initializer_list<path> inputs,
              language lang = language::cpp_latest,
              preprocessor_args const* pp_args = nullptr,
              flags const* flags = nullptr) {
    create(dst, inputs.begin(), inputs.size(), lang, pp_args, flags);
  }

  // TODO: Virtual constructors for preprocessor_args and flags

  virtual ~compiler_interface() noexcept = default;

private:
  // TODO: maybe make these public?
  virtual auto is_archive(path const& p) const noexcept -> bool = 0;
  virtual auto is_dynlib(path const& p) const noexcept -> bool = 0;
  virtual auto is_program(path const& p) const noexcept -> bool = 0;
};

} // namespace build_project

#endif