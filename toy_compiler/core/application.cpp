#include <toy_compiler/core/application.hpp>

#include <fmt/ranges.h>

#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

application::application(std::span<const std::string_view> args) : m_logger{"toy compiler"}
{
   for (const auto filename : args)
   {
      const auto filepath = fs::path{filename};
      if (filepath.extension() == ".src")
      {
         if (auto maybe = lex::tokenize_file(filepath))
         {
            write_tokens_to_file(filepath, maybe.value());
         }
         else
         {
            m_logger.warning(
               "Failed to open file located at \"{}\". Make sure the filepath is valid",
               filepath.c_str());
         }
      }
      else
      {
         m_logger.warning("File \"{}\" does not have the correct file extension (.src)",
                          filepath.c_str());
      }
   }
}

void application::write_tokens_to_file(const std::filesystem::path& path,
                                       std::span<const lex::token> tokens) const
{
   auto output_path = path.parent_path();
   output_path /= path.stem();
   output_path += ".outlextokens";

   std::string outlextoken;
   for (std::uint32_t line_counter = 1; const auto& tok : tokens)
   {
      for (std::uint32_t i = line_counter; i < tok.line; ++i)
      {
         outlextoken.append("\n");
      }

      outlextoken.append(fmt::format("{} ", tok));

      line_counter = tok.line;
   }

   std::ofstream output_file{output_path};
   output_file << outlextoken;
}

void application::write_errors_to_file(const std::filesystem::path& path,
                                       [[maybe_unused]] std::span<const lex::token> tokens) const
{
   auto output_path = path.parent_path();
   output_path /= path.stem();
   output_path += ".outlexerrors";
}
