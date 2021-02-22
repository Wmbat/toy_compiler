/**
 * @file logger.hpp
 * @brief Contains all the logger related utility
 */

#pragma once

#include <spdlog/spdlog.h>

#include <cstring>
#include <string_view>

namespace util
{
   class logger
   {
   public:
      logger();
      logger(std::string_view name);

      void debug(const std::string& msg);
      void info(const std::string& msg);
      void warning(const std::string& msg);
      void error(const std::string& msg);

      void flush();

      template <typename... Args>
      void info(std::string_view msg, Args&&... args)
      {
         log.info(msg, std::forward<Args>(args)...);
      }
      template <typename... Args>
      void debug(std::string_view msg, Args&&... args)
      {
         log.debug(msg, std::forward<Args>(args)...);
      }
      template <typename... Args>
      void warning(std::string_view msg, Args&&... args)
      {
         log.warn(msg, std::forward<Args>(args)...);
         log.flush();
      }
      template <typename... Args>
      void error(std::string_view msg, Args&&... args)
      {
         log.error(msg, std::forward<Args>(args)...);
         log.flush();
      }

      auto get_logger() -> spdlog::logger&;
      auto get_logger() const -> const spdlog::logger&;

   private:
      spdlog::logger log;
   };

   class logger_wrapper
   {
   public:
      logger_wrapper(util::logger* p_logger = nullptr);

      void debug(const std::string& msg);
      void info(const std::string& msg);
      void warning(const std::string& msg);
      void error(const std::string& msg);

      template <typename... Args>
      void info(std::string_view msg, Args&&... args)
      {
         if (mp_logger)
         {
            mp_logger->info(msg, std::forward<Args>(args)...);
         }
      }
      template <typename... Args>
      void debug(std::string_view msg, Args&&... args)
      {
         if (mp_logger)
         {
            mp_logger->debug(msg, std::forward<Args>(args)...);
         }
      }
      template <typename... Args>
      void warning(std::string_view msg, Args&&... args)
      {
         if (mp_logger)
         {
            mp_logger->warning(msg, std::forward<Args>(args)...);
         }
      }
      template <typename... Args>
      void error(std::string_view msg, Args&&... args)
      {
         if (mp_logger)
         {
            mp_logger->error(msg, std::forward<Args>(args)...);
         }
      }

      auto get() const -> util::logger*; // NOLINT

      [[nodiscard]] auto take() -> util::logger*;

   private:
      util::logger* mp_logger;
   };
} // namespace util
