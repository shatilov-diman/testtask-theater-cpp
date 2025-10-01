#include "types.hpp"

class Movie {
    guid_t guid;
    std::string name;
    std::string description;
  public:
    Movie(const guid_t& guid, const std::string& name, const std::string& description)
    : guid(guid), name(name), description(description) {
    }
    ~Movie() {
    }

  public:
    const guid_t& get_guid() const {
      return guid;
    }
    const std::string& get_name() const {
      return name;
    }
    const std::string& get_description() const {
      return description;
    }
}
