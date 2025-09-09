include(cmake/SystemLink.cmake)
include(cmake/LibFuzzer.cmake)
include(CMakeDependentOption)
include(CheckCXXCompilerFlag)


include(CheckCXXSourceCompiles)


macro(chroma_supports_sanitizers)
  if((CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*") AND NOT WIN32)

    message(STATUS "Sanity checking UndefinedBehaviorSanitizer, it should be supported on this platform")
    set(TEST_PROGRAM "int main() { return 0; }")

    # Check if UndefinedBehaviorSanitizer works at link time
    set(CMAKE_REQUIRED_FLAGS "-fsanitize=undefined")
    set(CMAKE_REQUIRED_LINK_OPTIONS "-fsanitize=undefined")
    check_cxx_source_compiles("${TEST_PROGRAM}" HAS_UBSAN_LINK_SUPPORT)

    if(HAS_UBSAN_LINK_SUPPORT)
      message(STATUS "UndefinedBehaviorSanitizer is supported at both compile and link time.")
      set(SUPPORTS_UBSAN ON)
    else()
      message(WARNING "UndefinedBehaviorSanitizer is NOT supported at link time.")
      set(SUPPORTS_UBSAN OFF)
    endif()
  else()
    set(SUPPORTS_UBSAN OFF)
  endif()

  if((CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*") AND WIN32)
    set(SUPPORTS_ASAN OFF)
  else()
    if (NOT WIN32)
      message(STATUS "Sanity checking AddressSanitizer, it should be supported on this platform")
      set(TEST_PROGRAM "int main() { return 0; }")

      # Check if AddressSanitizer works at link time
      set(CMAKE_REQUIRED_FLAGS "-fsanitize=address")
      set(CMAKE_REQUIRED_LINK_OPTIONS "-fsanitize=address")
      check_cxx_source_compiles("${TEST_PROGRAM}" HAS_ASAN_LINK_SUPPORT)

      if(HAS_ASAN_LINK_SUPPORT)
        message(STATUS "AddressSanitizer is supported at both compile and link time.")
        set(SUPPORTS_ASAN ON)
      else()
        message(WARNING "AddressSanitizer is NOT supported at link time.")
        set(SUPPORTS_ASAN OFF)
      endif()
    else()
      set(SUPPORTS_ASAN ON)
    endif()
  endif()
endmacro()

macro(chroma_setup_options)
  option(chroma_ENABLE_HARDENING "Enable hardening" ON)
  option(chroma_ENABLE_COVERAGE "Enable coverage reporting" OFF)
  cmake_dependent_option(
    chroma_ENABLE_GLOBAL_HARDENING
    "Attempt to push hardening options to built dependencies"
    ON
    chroma_ENABLE_HARDENING
    OFF)

  chroma_supports_sanitizers()

  if(NOT PROJECT_IS_TOP_LEVEL OR chroma_PACKAGING_MAINTAINER_MODE)
    option(chroma_ENABLE_IPO "Enable IPO/LTO" OFF)
    option(chroma_WARNINGS_AS_ERRORS "Treat Warnings As Errors" OFF)
    option(chroma_ENABLE_USER_LINKER "Enable user-selected linker" OFF)
    option(chroma_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" OFF)
    option(chroma_ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
    option(chroma_ENABLE_SANITIZER_UNDEFINED "Enable undefined sanitizer" OFF)
    option(chroma_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
    option(chroma_ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)
    option(chroma_ENABLE_UNITY_BUILD "Enable unity builds" OFF)
    option(chroma_ENABLE_CLANG_TIDY "Enable clang-tidy" OFF)
    option(chroma_ENABLE_CPPCHECK "Enable cpp-check analysis" OFF)
    option(chroma_ENABLE_PCH "Enable precompiled headers" OFF)
    option(chroma_ENABLE_CACHE "Enable ccache" OFF)
  else()
    option(chroma_ENABLE_IPO "Enable IPO/LTO" ON)
    option(chroma_WARNINGS_AS_ERRORS "Treat Warnings As Errors" ON)
    option(chroma_ENABLE_USER_LINKER "Enable user-selected linker" OFF)
    option(chroma_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" ${SUPPORTS_ASAN})
    option(chroma_ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
    option(chroma_ENABLE_SANITIZER_UNDEFINED "Enable undefined sanitizer" ${SUPPORTS_UBSAN})
    option(chroma_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
    option(chroma_ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)
    option(chroma_ENABLE_UNITY_BUILD "Enable unity builds" OFF)
    option(chroma_ENABLE_CLANG_TIDY "Enable clang-tidy" ON)
    option(chroma_ENABLE_CPPCHECK "Enable cpp-check analysis" ON)
    option(chroma_ENABLE_PCH "Enable precompiled headers" OFF)
    option(chroma_ENABLE_CACHE "Enable ccache" ON)
  endif()

  if(NOT PROJECT_IS_TOP_LEVEL)
    mark_as_advanced(
      chroma_ENABLE_IPO
      chroma_WARNINGS_AS_ERRORS
      chroma_ENABLE_USER_LINKER
      chroma_ENABLE_SANITIZER_ADDRESS
      chroma_ENABLE_SANITIZER_LEAK
      chroma_ENABLE_SANITIZER_UNDEFINED
      chroma_ENABLE_SANITIZER_THREAD
      chroma_ENABLE_SANITIZER_MEMORY
      chroma_ENABLE_UNITY_BUILD
      chroma_ENABLE_CLANG_TIDY
      chroma_ENABLE_CPPCHECK
      chroma_ENABLE_COVERAGE
      chroma_ENABLE_PCH
      chroma_ENABLE_CACHE)
  endif()

  chroma_check_libfuzzer_support(LIBFUZZER_SUPPORTED)
  if(LIBFUZZER_SUPPORTED AND (chroma_ENABLE_SANITIZER_ADDRESS OR chroma_ENABLE_SANITIZER_THREAD OR chroma_ENABLE_SANITIZER_UNDEFINED))
    set(DEFAULT_FUZZER ON)
  else()
    set(DEFAULT_FUZZER OFF)
  endif()

  option(chroma_BUILD_FUZZ_TESTS "Enable fuzz testing executable" ${DEFAULT_FUZZER})

endmacro()

macro(chroma_global_options)
  if(chroma_ENABLE_IPO)
    include(cmake/InterproceduralOptimization.cmake)
    chroma_enable_ipo()
  endif()

  chroma_supports_sanitizers()

  if(chroma_ENABLE_HARDENING AND chroma_ENABLE_GLOBAL_HARDENING)
    include(cmake/Hardening.cmake)
    if(NOT SUPPORTS_UBSAN 
       OR chroma_ENABLE_SANITIZER_UNDEFINED
       OR chroma_ENABLE_SANITIZER_ADDRESS
       OR chroma_ENABLE_SANITIZER_THREAD
       OR chroma_ENABLE_SANITIZER_LEAK)
      set(ENABLE_UBSAN_MINIMAL_RUNTIME FALSE)
    else()
      set(ENABLE_UBSAN_MINIMAL_RUNTIME TRUE)
    endif()
    message("${chroma_ENABLE_HARDENING} ${ENABLE_UBSAN_MINIMAL_RUNTIME} ${chroma_ENABLE_SANITIZER_UNDEFINED}")
    chroma_enable_hardening(chroma_options ON ${ENABLE_UBSAN_MINIMAL_RUNTIME})
  endif()
endmacro()

macro(chroma_local_options)
  if(PROJECT_IS_TOP_LEVEL)
    include(cmake/StandardProjectSettings.cmake)
  endif()

  add_library(chroma_warnings INTERFACE)
  add_library(chroma_options INTERFACE)

  include(cmake/CompilerWarnings.cmake)
  chroma_set_project_warnings(
    chroma_warnings
    ${chroma_WARNINGS_AS_ERRORS}
    ""
    ""
    ""
    "")

  if(chroma_ENABLE_USER_LINKER)
    include(cmake/Linker.cmake)
    chroma_configure_linker(chroma_options)
  endif()

  include(cmake/Sanitizers.cmake)
  chroma_enable_sanitizers(
    chroma_options
    ${chroma_ENABLE_SANITIZER_ADDRESS}
    ${chroma_ENABLE_SANITIZER_LEAK}
    ${chroma_ENABLE_SANITIZER_UNDEFINED}
    ${chroma_ENABLE_SANITIZER_THREAD}
    ${chroma_ENABLE_SANITIZER_MEMORY})

  set_target_properties(chroma_options PROPERTIES UNITY_BUILD ${chroma_ENABLE_UNITY_BUILD})

  if(chroma_ENABLE_PCH)
    target_precompile_headers(
      chroma_options
      INTERFACE
      <vector>
      <string>
      <utility>)
  endif()

  if(chroma_ENABLE_CACHE)
    include(cmake/Cache.cmake)
    chroma_enable_cache()
  endif()

  include(cmake/StaticAnalyzers.cmake)
  if(chroma_ENABLE_CLANG_TIDY)
    chroma_enable_clang_tidy(chroma_options ${chroma_WARNINGS_AS_ERRORS})
  endif()

  if(chroma_ENABLE_CPPCHECK)
    chroma_enable_cppcheck(${chroma_WARNINGS_AS_ERRORS} "" # override cppcheck options
    )
  endif()

  if(chroma_ENABLE_COVERAGE)
    include(cmake/Tests.cmake)
    chroma_enable_coverage(chroma_options)
  endif()

  if(chroma_WARNINGS_AS_ERRORS)
    check_cxx_compiler_flag("-Wl,--fatal-warnings" LINKER_FATAL_WARNINGS)
    if(LINKER_FATAL_WARNINGS)
      # This is not working consistently, so disabling for now
      # target_link_options(chroma_options INTERFACE -Wl,--fatal-warnings)
    endif()
  endif()

  if(chroma_ENABLE_HARDENING AND NOT chroma_ENABLE_GLOBAL_HARDENING)
    include(cmake/Hardening.cmake)
    if(NOT SUPPORTS_UBSAN 
       OR chroma_ENABLE_SANITIZER_UNDEFINED
       OR chroma_ENABLE_SANITIZER_ADDRESS
       OR chroma_ENABLE_SANITIZER_THREAD
       OR chroma_ENABLE_SANITIZER_LEAK)
      set(ENABLE_UBSAN_MINIMAL_RUNTIME FALSE)
    else()
      set(ENABLE_UBSAN_MINIMAL_RUNTIME TRUE)
    endif()
    chroma_enable_hardening(chroma_options OFF ${ENABLE_UBSAN_MINIMAL_RUNTIME})
  endif()

endmacro()
