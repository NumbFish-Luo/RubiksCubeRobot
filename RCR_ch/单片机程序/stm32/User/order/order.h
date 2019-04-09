#ifndef ORDER_H
#define ORDER_H

#include "stm32f4xx.h"

void Delay(__IO uint32_t nCount);

typedef struct Order {
    const char* name;
    void(*Do)(void);
} Order;
Order NewOrder(void);

typedef struct Invoker Invoker;
struct Invoker {
    Order order;
    void(*Clear)(Invoker* self);
    void(*DoOrder)(Invoker* self);
    void(*SetOrder)(Invoker* self, Order order);
};
Invoker NewInvoker(void);

#define NEW_ORDER_decl(_) Order##_ NewOrder##_(void)
#define CREATE_ORDER(_)       \
    typedef struct Order##_ { \
        Order base;           \
    } Order##_;               \
    NEW_ORDER_decl(_);

CREATE_ORDER(ERR); // Error
CREATE_ORDER(LMA); // LM+
CREATE_ORDER(LMS); // LM-
CREATE_ORDER(LMF); // LMF
CREATE_ORDER(LMR); // LMR
CREATE_ORDER(LM2); // LM2
CREATE_ORDER(LCA); // LC+
CREATE_ORDER(LCS); // LC-
CREATE_ORDER(DMA); // DM+
CREATE_ORDER(DMS); // DM-
CREATE_ORDER(DMF); // DMF
CREATE_ORDER(DMR); // DMR
CREATE_ORDER(DM2); // DM2
CREATE_ORDER(DCA); // DC+
CREATE_ORDER(DCS); // DC-
CREATE_ORDER(ACA); // AC+
CREATE_ORDER(ACS); // AC-

#endif // ORDER_H
