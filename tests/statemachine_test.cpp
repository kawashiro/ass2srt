#include "statemachine.hpp"
#include <gtest/gtest.h>
#include <memory>

using namespace ass2srt;

class TestState2 : public statemachine::State<int> {
public:
    std::unique_ptr<statemachine::State<int>> transition(int&) override
    {
        return std::make_unique<statemachine::FinalState<int>>();
    }

    void output(int& value) override
    {
        value += 10;
    }
};

class TestState1 : public statemachine::State<int> {
public:
    std::unique_ptr<statemachine::State<int>> transition(int&) override
    {
        return std::make_unique<TestState2>();
    }

    void output(int& value) override
    {
        value++;
    }
};

TEST(StateMachine, TestMainLoop)
{
    auto* init_state = new TestState1;
    int init_value = 42;
    statemachine::run<int>(init_state, init_value);
    ASSERT_EQ(init_value, 53);
}
