#include <stdio.h>
#include <hippo_kv.hpp>
int main() {
    HIPPOKV db("kvdb.log");
    db.hippo_put("key2", "test");
    printf("hello");
    return 0;
}
