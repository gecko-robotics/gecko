#include <iostream>
#include <map>
#include <string>

#include <spotify/json.hpp>

using namespace spotify::json;

struct Track {
  std::string uri;
  std::string uid;
  std::map<std::string, std::string> metadata;
};

namespace spotify {
namespace json {

// Specialize spotify::json::default_codec_t to specify default behavior when
// encoding and decoding objects of certain types.
template <>
struct default_codec_t<Track> {
  static object_t<Track> codec() {
    auto codec = object<Track>();
    codec.required("uri", &Track::uri);
    codec.optional("uid", &Track::uid);
    codec.optional("metadata", &Track::metadata);
    return codec;
  }
};

}  // namespace json
}  // namespace spotify

int main() {
  const auto parsed_track = decode<Track>(R"({ "uri": "spotify:track:xyz", "metadata": { "a": "b" } })");
  std::cout << "Parsed track with uri " << parsed_track.uri << std::endl;

  Track track;
  track.uri = "spotify:track:abc";
  track.uid = "a-uid";
  const auto json = encode(track);
  std::cout << "Encoded the track into " << json << std::endl;

  return 0;
}
