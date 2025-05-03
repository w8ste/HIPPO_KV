#include <stdio.h>
#include <hippo_kv.hpp>
int main() {
    HIPPOKV db("kvdb.log");
    db.hippo_put("tsfasfsfsfsfsdfsd", "mainsdfsfsdfsdf");
    //db.hippo_delete("tag2");
    //    db.hippo_delete("tag1");
    printf("----Finished------\n");
    return 0;
}
