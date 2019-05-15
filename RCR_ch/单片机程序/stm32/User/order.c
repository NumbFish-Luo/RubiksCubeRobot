#include "order.h"
#include "SerialPort.h"
#include "Init.h"
#include "StepMotor.h"

#define DELAY_CLAMP  (uint32_t)(4000000)
#define DELAY_LOOSEN (uint32_t)(4500000)

void OrderDo() {
    printf("[DoBase]\n");
}
Order NewOrder() {
    Order o;
    o.name = "Order";
    o.Do   = OrderDo;
    return o;
}

void InvokerClear(Invoker* i) { }
void InvokerDoOrder(Invoker* i) {
    i->order.Do();
}
void InvokerSetOrder(Invoker* i, Order o) {
    i->order = o;
}
Invoker NewInvoker() {
    Invoker i;
    i.order     = NewOrder();
    i.Clear     = InvokerClear;
    i.DoOrder   = InvokerDoOrder;
    i.SetOrder  = InvokerSetOrder;
    return i;
}

#define NEW_ORDER_impl(_, _name_) \
    NEW_ORDER_decl(_) {            \
        Order##_ o;                 \
        o.base.name = _name_;        \
        o.base.Do   = Order##_##Do;   \
        return o;                      \
    }
#define ORDER_DO(_, _name_)  \
    void Order##_##Do(void);  \
    NEW_ORDER_impl(_, _name_); \
    void Order##_##Do(void)

typedef enum HandType {
    Left = 0, Down = 1
}HandType;
    
void SpeedAdjust(HandType handType) {
    if (handType == Left) {
        if (g_robotState.d_hand == Loosen) { // 如果对面手是松开的，那么应该慢慢转
            g_robotState.speed = Slow;
        } else {
            g_robotState.speed = Fast;
        }
    } else {
        if (g_robotState.l_hand == Loosen) { // 如果对面手是松开的，那么应该慢慢转
            g_robotState.speed = Slow;
        } else {
            g_robotState.speed = Fast;
        }
    }
}

ORDER_DO(ERR, "ERR") {
    printf("[ERR] ");
}
ORDER_DO(LMA, "LM+") {
    printf("[LM+] ");
    SpeedAdjust(Left); // 速度调整
    motor_L.Turn(&motor_L, TURN_90);
}
ORDER_DO(LMS, "LM-") {
    printf("[LM-] ");
    SpeedAdjust(Left); // 速度调整
    motor_L.Turn(&motor_L, -TURN_90);
}
ORDER_DO(LMF, "LMF") {
    printf("[LMF] ");
    motor_L.Turn(&motor_L, TURN_1);
}
ORDER_DO(LMR, "LMR") {
    printf("[LMR] ");
    motor_L.Turn(&motor_L, -TURN_1);
}
ORDER_DO(LM2, "LM2") {
    printf("[LM2] ");
    SpeedAdjust(Left); // 速度调整
    motor_L.Turn(&motor_L, TURN_180);
}
ORDER_DO(LMT, "LMT") {
    printf("[LMT] ");
    SpeedAdjust(Left); // 速度调整
    motor_L.Turn(&motor_L, -TURN_180);
}
ORDER_DO(LCA, "LC+") {
    printf("[LC+] ");
    g_robotState.l_hand = Clamp; // 左侧夹紧
    claw_L.ON(&claw_L);
    Delay(DELAY_CLAMP);
}
ORDER_DO(LCS, "LC-") {
    printf("[LC-] ");
    g_robotState.l_hand = Loosen; // 左侧松开
    claw_L.OFF(&claw_L);
    Delay(DELAY_LOOSEN);
}
ORDER_DO(DMA, "DM+") {
    printf("[DM+] ");
    SpeedAdjust(Down); // 速度调整
    motor_D.Turn(&motor_L, TURN_90);
}
ORDER_DO(DMS, "DM-") {
    printf("[DM-] ");
    SpeedAdjust(Down); // 速度调整
    motor_D.Turn(&motor_L, -TURN_90);
}
ORDER_DO(DMF, "DMF") {
    printf("[DMF] ");
    motor_D.Turn(&motor_L, TURN_1);
}
ORDER_DO(DMR, "DMR") {
    printf("[DMR] ");
    motor_D.Turn(&motor_L, -TURN_1);
}
ORDER_DO(DM2, "DM2") {
    printf("[DM2] ");
    SpeedAdjust(Down); // 速度调整
    motor_D.Turn(&motor_L, TURN_180);
}
ORDER_DO(DMT, "DMT") {
    printf("[DMT] ");
    SpeedAdjust(Down); // 速度调整
    motor_D.Turn(&motor_L, -TURN_180);
}
ORDER_DO(DCA, "DC+") {
    printf("[DC+] ");
    g_robotState.d_hand = Clamp; // 下侧夹紧
    claw_D.ON(&claw_D);
    Delay(DELAY_CLAMP);
}
ORDER_DO(DCS, "DC-") {
    printf("[DC-] ");
    g_robotState.d_hand = Loosen; // 下侧松开
    claw_D.OFF(&claw_D);
    Delay(DELAY_LOOSEN);
}
ORDER_DO(ACA, "AC+") {
    printf("[AC+] ");
    claw_L.ON(&claw_L);
    claw_D.ON(&claw_D);
    Delay(DELAY_CLAMP);
}
ORDER_DO(ACS, "AC-") {
    printf("[AC-] ");
    claw_L.OFF(&claw_L);
    claw_D.OFF(&claw_D);
    Delay(DELAY_LOOSEN);
}
