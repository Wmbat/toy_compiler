/**
 * @file lexer.hpp
 * @brief Contains all the lexing functions for the compiler
 */

#pragma once

#include <toy_compiler/lex/item.hpp>
#include <toy_compiler/util/logger.hpp>

#include <libcaramel/containers/dynamic_array.hpp>

#include <monads/maybe.hpp>

#include <filesystem>

namespace lex
{
   /**
    * @brief Opens a file and lexes it
    *
    * @param path The path to the file
    * @param log A logger used for status information
    *
    * @return The list of `lex::item` from the file
    */
   auto lex_file(const std::filesystem::path& path, util::logger_wrapper log = nullptr)
      -> monad::maybe<crl::dynamic_array<lex::item>>;
} // namespace lex
