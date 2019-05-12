#include <stdlib.h>
#include <string.h>
#include "order/Orders.h"

VectorOrder g_orders;
RobotState g_robotState = { Clamp, Clamp, Fast };

void VectorOrderClear(VectorOrder* v) {
    free(v->datas);
    v->datas    = 0;
    v->dataSize = 0;
    v->maxSize  = 0;
}
Order VectorOrderFind(VectorOrder* v, const char* name) {
    size_t i = 0;
    for (i = 0; i < v->dataSize; ++i) {
        if (strcmp(v->datas[i].name, name) == 0) {
            return v->datas[i];
        }
    }
    return NewOrderERR().base;
}
void VectorOrderPushBack(VectorOrder* v, Order o) {
    Order* tmp = 0;
    size_t i = 0;
    if ((int)v->maxSize - (int)v->dataSize <= 1) {
        v->maxSize = 2 * v->maxSize + 1;
        tmp = (Order*)malloc(v->maxSize * sizeof(Order));
        for (i = 0; i < v->dataSize; ++i) {
            tmp[i] = v->datas[i];
        }
        free(v->datas);
        v->datas = tmp;
    }
    v->datas[v->dataSize++] = o;
}
void VectorOrderSetData(VectorOrder* v, Order* o, size_t count) {
    Order* tmp = 0;
    if (o == 0) {
        return;
    }
    if (count == 0) {
        v->Clear(v);
        return;
    }
    v->dataSize = count;
    v->maxSize = 2 * count;
    tmp = (Order*)malloc(v->maxSize * sizeof(Order));
    for (size_t i = 0; i < count; ++i) {
        tmp[i] = o[i];
    }
    free(v->datas);
    v->datas = tmp;
}
size_t VectorOrderSize(VectorOrder* v) {
    return v->dataSize;
}
VectorOrder NewVectorOrder() {
    VectorOrder v;
    v.datas    = 0;
    v.dataSize = 0;
    v.maxSize  = 0;
    v.Clear    = VectorOrderClear;
    v.Find     = VectorOrderFind;
    v.PushBack = VectorOrderPushBack;
    v.SetDatas = VectorOrderSetData;
    v.Size     = VectorOrderSize;
    return v;
}

void ArrayOrderClear(ArrayOrder* a) {
    free(a->datas);
    a->datas = 0;
    a->size  = 0;
}
Order ArrayOrderFind(ArrayOrder* a, const char* name) {
    size_t i = 0;
    for (i = 0; i < a->size; ++i) {
        if (strcmp(a->datas[i].name, name) == 0) {
            return a->datas[i];
        }
    }
    return NewOrderERR().base;
}
void ArrayOrderSet(ArrayOrder* a, size_t index, Order data) {
    a->datas[index] = data;
}
ArrayOrder NewArrayOrder(size_t count) {
    ArrayOrder a;
    a.datas = (Order*)malloc(count * sizeof(Order));
    a.size  = count;
    a.Clear = ArrayOrderClear;
    a.Find  = ArrayOrderFind;
    a.Set   = ArrayOrderSet;
    return a;
}
