#ifndef KVSTORE_HPP
#define KVSTORE_HPP

#include <string>
#include <unordered_map>
#include <fstream>



class HIPPOKV {
  public:
  HIPPOKV(const std::string& file_name);

  ~HIPPOKV();

  // Insert key value pair overwriting existing value
  void hippo_put(const std::string& key, const std::string& value);

  // Insert key value pair not overwriting existing value
  void hippo_put_shallow();

  bool hippo_get();

private:
  std::string file_name;
  std::fstream file;
  std::unordered_map<std::string, std::string> db_map;

  void replay_log();
  
};

#endif
