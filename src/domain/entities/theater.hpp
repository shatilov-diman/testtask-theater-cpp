#include "types.hpp"

class Theater {
    std::vector<guid_t> movies_guids;
  public:
    Theater() {
    }
    ~Theater() {
    }
  
  public:
    void add_movie(const guid_t& guid) {
      movies_guids.push_back(guid);
    }

    void remove_movie(const guid_t& guid) {
      movies_guids.erase(std::remove(movies_guids.begin(), movies_guids.end(), guid), movies_guids.end());
    }

    const std::vector<guid_t>& get_movies_guids() const {
      return movies_guids;
    }
};
