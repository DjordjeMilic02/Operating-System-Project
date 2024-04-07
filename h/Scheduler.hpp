#ifndef PROJECT_BASE_V1_1_SCHEDULER_HPP
#define PROJECT_BASE_V1_1_SCHEDULER_HPP

class TCB;
struct node
{
    TCB *elem;
    node *next;
};

class Scheduler
{
private:
    static node* head;
    static node* tail;

public:
    static Scheduler& getInstance();
    TCB* get();
    void put (TCB* t);

};

#endif
