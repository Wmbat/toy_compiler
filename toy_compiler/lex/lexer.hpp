#pragma once

#include <toy_compiler/lex/token.hpp>
#include <toy_compiler/util/logger.hpp>

#include <libcaramel/containers/dynamic_array.hpp>

#include <monads/maybe.hpp>

#include <filesystem>

namespace lex
{
   /**
    * @brief Opens a file and tokenize it.
    *
    * @param path, The path to the file.
    * @param log, A logger used for status information
    *
    * @return The list of tokens.
    */
   auto tokenize_file(const std::filesystem::path& path, util::logger_wrapper log = nullptr)
      -> monad::maybe<crl::dynamic_array<token>>;
} // namespace lex
