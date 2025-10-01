#pragma once

#include "types.hpp"

class Theater {
    theater_guid_t m_guid;
    std::string m_name;
    std::string m_description;

    std::set<movie_guid_t> m_movies_guids;
  public:
    Theater(const theater_guid_t& guid, const std::string& name, const std::string& description, const std::set<movie_guid_t>& movies_guids)
    : m_guid(guid), m_name(name), m_description(description), m_movies_guids(movies_guids) {
    }
    ~Theater() {
    }
  
  public:
    const theater_guid_t& get_guid() const {
      return m_guid;
    }
    const std::string& get_name() const {
      return m_name;
    }
    const std::string& get_description() const {
      return m_description;
    }

    std::set<movie_guid_t> get_movies_guids() const {
      return m_movies_guids;
    }
};
