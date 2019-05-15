#ifndef MAP_NAME_ORDER_H
#define MAP_NAME_ORDER_H

#include "Order.h"

typedef unsigned int size_t;
typedef struct VectorOrder VectorOrder;
struct VectorOrder {
    Order* datas;
    size_t dataSize;
    size_t maxSize;
    void(*Clear)(VectorOrder* self);
    Order(*Find)(VectorOrder* self, const char* name);
    void(*PushBack)(VectorOrder* self, Order data);
    void(*SetDatas)(VectorOrder* self, Order* datas, size_t count);
    size_t(*Size)(VectorOrder* self);
};
VectorOrder NewVectorOrder(void);

typedef struct ArrayOrder ArrayOrder;
struct ArrayOrder {
    Order* datas;
    size_t size;
    void(*Clear)(ArrayOrder* self);
    Order(*Find)(ArrayOrder* self, const char* name);
    void(*Set)(ArrayOrder* self, size_t index, Order data);
};
ArrayOrder NewArrayOrder(size_t count);

extern VectorOrder g_orders;

#endif // !MAP_NAME_ORDER_H
