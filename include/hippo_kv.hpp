#ifndef KVSTORE_HPP
#define KVSTORE_HPP

#include <string>
#include <unordered_map>
#include <fstream>
#include <cstdint>


class HIPPOKV {
  public:
  HIPPOKV(const std::string& file_name);

  ~HIPPOKV();

  // Insert key value pair overwriting existing value
  void hippo_put(const std::string& key, const std::string& value);

  // Insert key value pair not overwriting existing value
  void hippo_put_shallow();

  bool hippo_get(const std::string& key, std::string& value);

  void hippo_delete(const std::string& key);

private:
  std::string file_name;
  std::fstream file;
  std::unordered_map<std::string, std::string> db_map;

  void replay_log();

  void hippo_write_to_db(uint8_t op, const std::string& key, const std::string& value);
};

#endif
