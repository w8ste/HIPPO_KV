#include "hippo_kv.hpp"
#include <iostream>

enum op_code {PUT = (uint8_t)0x01, PUT_SHALLOW = (uint8_t)0x02, DEL = (uint8_t)0x03};


HIPPOKV::HIPPOKV(const std::string& path) : db_path(path) {
    replay_log();  // Read old data
    out_file.open(db_path, std::ios::binary | std::ios::app);
    if (!out_file) {
        std::cerr << "Failed to open output file\n";
    }
}

HIPPOKV::~HIPPOKV() {
    out_file.close();
}

void HIPPOKV::hippo_put(const std::string& key, const std::string& value) {
    uint8_t op = PUT;
    uint32_t key_size = key.size();
    uint32_t value_size = value.size();

    out_file.write(reinterpret_cast<const char*>(&op), sizeof(op));
    out_file.write(reinterpret_cast<const char*>(&key_size), sizeof(key_size));
    out_file.write(key.c_str(), key_size);
    out_file.write(reinterpret_cast<const char*>(&value_size), sizeof(value_size));
    out_file.write(value.c_str(), value_size);
    out_file.flush();

    db_map[key] = value;
}

void HIPPOKV::replay_log() {
    std::ifstream in_file(db_path, std::ios::binary);
    if (!in_file) return;

    while (true) {
        uint8_t op;
        uint32_t key_size, value_size;

        in_file.read(reinterpret_cast<char*>(&op), sizeof(op));
        if (!in_file) break;

        in_file.read(reinterpret_cast<char*>(&key_size), sizeof(key_size));
        in_file.read(reinterpret_cast<char*>(&value_size), sizeof(value_size));
        if (!in_file) break;

        std::string key(key_size, '\0');
        std::string value(value_size, '\0');

        in_file.read(&key[0], key_size);
        in_file.read(&value[0], value_size);
        if (!in_file) break;

        if (op == PUT) db_map[key] = value;
        else if (op == DEL) db_map.erase(key);
    }
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

  out_file.write(reinterpret_cast<const char*>(&op), sizeof(op));

  out_file.write(reinterpret_cast<const char*>(&key_size), sizeof(key_size));
  out_file.write(key.c_str(), key_size);

  out_file.write(reinterpret_cast <const char*>(&value_size), sizeof(value_size));
  out_file.write(value.c_str(), value_size);
}

bool HIPPOKV::hippo_get(const std::string& key, std::string& value) {
  auto it = db_map.find(key);

  if(it != db_map.end()) {
    value = it->second;
    return true;
  }
  return false;
}


