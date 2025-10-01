#pragma once

#include "types.hpp"

class Movie {
    movie_guid_t m_guid;
    std::string m_name;
    std::string m_description;
  public:
    Movie(const movie_guid_t& guid, const std::string& name, const std::string& description)
    : m_guid(guid), m_name(name), m_description(description) {
    }
    ~Movie() {
    }

  public:
    const movie_guid_t& get_guid() const {
      return m_guid;
    }
    const std::string& get_name() const {
      return m_name;
    }
    const std::string& get_description() const {
      return m_description;
    }
};
