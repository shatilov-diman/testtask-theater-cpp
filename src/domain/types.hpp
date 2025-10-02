#pragma once

#include <string>
#include <map>
#include <set>

#include <strong_type/strong_type.hpp>

template<typename T, typename Tag>
using guid_t = strong::type<T, Tag, strong::ordered, strong::hashable, strong::formattable, strong::iostreamable>;

using movie_guid_t = guid_t<std::string, struct movie_guid_tag>;
using theater_guid_t = guid_t<std::string, struct theater_guid_tag>;
using seat_guid_t = guid_t<std::string, struct seat_guid_tag>;

using error_message_t = strong::type<std::string, struct error_message_tag, strong::ordered, strong::hashable, strong::formattable, strong::iostreamable>;