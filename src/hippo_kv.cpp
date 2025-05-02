#include "hippo_kv.hpp"
#include <iostream>

enum op_code {PUT = (uint8_t)0x01, PUT_SHALLOW = (uint8_t)0x02, DEL = (uint8_t)0x03};

HIPPOKV::HIPPOKV(const std::string& file_name) : file_name(file_name) {
  file.open(file_name, std::ios::binary | std::ios::in | std::ios::out | std::ios::app);

  if(!file) {
    std::cerr << "Failed to open db file!" << std::endl;
  }

  //replay_log();

}

HIPPOKV::~HIPPOKV() {
  if(file) {
    file.close();
  }
}

void HIPPOKV::hippo_put(const std::string& key, const std::string& value) {

  if (!file.is_open()) {
    std::cerr << "File is not open!\n";
    return;
}

  file.seekp(0, std::ios::end);

  op_code op = PUT;
  uint32_t key_size = key.size();
  uint32_t value_size = value.size();
  
  file.write(reinterpret_cast<const char*>(&op), sizeof(op));
  file.write(reinterpret_cast<const char*>(&key_size), sizeof(key_size));
  file.write(reinterpret_cast<const char*>(&value_size), sizeof(value_size));
  file.write(key.data(), key_size);
  file.write(value.data(), value_size);
  file.flush();
  db_map[key] = value;
}

void HIPPOKV::hippo_delete(const std::string& key) {
  op_code op = DEL;
  const std::string& value = "\0";

  hippo_write_to_db(op, key, value);

  db_map.erase(key);
}

void HIPPOKV::hippo_write_to_db(uint8_t op, const std::string &key,
                                const std::string &value) {
  uint32_t key_size = key.size();
  uint32_t value_size = value.size();

  file.write(reinterpret_cast<const char*>(&op), sizeof(op));

  file.write(reinterpret_cast<const char*>(&key_size), sizeof(key_size));
  file.write(key.c_str(), key_size);

  file.write(reinterpret_cast <const char*>(&value_size), sizeof(value_size));
  file.write(value.c_str(), value_size);
}

bool HIPPOKV::hippo_get(const std::string& key, std::string& value) {
  auto it = db_map.find(key);

  if(it != db_map.end()) {
    value = it->second;
    return true;
  }
  return false;
}


void HIPPOKV::replay_log() {

  while(file) {

    uint8_t op;
    uint32_t key_size;
    uint32_t value_size;

    file.read(reinterpret_cast<char*>(&op), sizeof(op));

    if (file.eof()) break;

    file.read(reinterpret_cast<char*>(&key_size), sizeof(key_size));
    file.read(reinterpret_cast<char*>(&value_size), sizeof(value_size));

    std::string key(key_size, '\0');
    std::string value(value_size, '\0');
          

    file.read(&key[0], sizeof(key_size));
    file.read(&value[0], sizeof(value_size));


    if (op == PUT) {
      db_map[key] = value;
    } else if (op == DEL) {
      db_map.erase(key);
    }
  }
}
