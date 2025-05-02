#include "hippo_kv.hpp"
#include <iostream>
#include <cstdint>

enum op_code {PUT = (uint8_t)0x01, PUT_SHALLOW = (uint8_t)0x02, GET = (uint8_t)0x03};

HIPPOKV::HIPPOKV(const std::string& file_name) : file_name(file_name) {
  file.open(file_name, std::ios::binary | std::ios::in | std::ios::out | std::ios::app);

  if(!file) {
    std::cerr << "Failed to open db file!" << std::endl;
  }

  replay_log();

}

HIPPOKV::~HIPPOKV() {
  if(file) {
    file.close();
  }
}

void HIPPOKV::hippo_put(const std::string& key, const std::string& value) {
  op_code op = PUT;
  uint32_t key_size = key.size();
  uint32_t value_size = value.size();
  
  file.write(reinterpret_cast<const char*>(&op), sizeof(op));
  file.write(reinterpret_cast<const char*>(&key_size), sizeof(key_size));
  file.write(key.c_str(), key_size);
  file.write(reinterpret_cast<const char*>(&value_size), sizeof(value_size));
  file.write(value.c_str(), value_size);

  db_map[key] = value;
}

void HIPPOKV::replay_log() {}
