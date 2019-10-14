#include <msgpack.h>
#include <msgpack/zbuffer.h>
#include <stdio.h>

void print(char const* buf, unsigned int len)
{
    size_t i = 0;
    for(; i < len ; ++i)
        printf("%02x ", 0xff & buf[i]);
    printf("\n");
}

int main(void)
{
    msgpack_zbuffer zbuf;
    msgpack_packer pk;
    msgpack_zone mempool;
    msgpack_object deserialized;

    msgpack_zbuffer_init(&zbuf,Z_NO_COMPRESSION,256);

    /* serialize values into the buffer using msgpack_zbuffer_write callback function. */
    msgpack_packer_init(&pk, &zbuf, msgpack_zbuffer_write);

    msgpack_pack_array(&pk, 3);
    msgpack_pack_int(&pk, 1);
    msgpack_pack_true(&pk);
    msgpack_pack_str(&pk, 7);
    msgpack_pack_str_body(&pk, "example", 7);

    print(zbuf.data, zbuf.init_size);

    /* deserialize the buffer into msgpack_object instance. */
    /* deserialized object is valid during the msgpack_zone instance alive. */
    msgpack_zone_init(&mempool, 2048);

    msgpack_unpack(zbuf.data, zbuf.init_size, NULL, &mempool, &deserialized);

    /* print the deserialized object. */
    msgpack_object_print(stdout, deserialized);
    puts("");

    msgpack_zone_destroy(&mempool);
    msgpack_zbuffer_destroy(&zbuf);

    return 0;
}
