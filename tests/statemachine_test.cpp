#include <gtest/gtest.h>
#include <memory>
#include "statemachine.hpp"

using namespace ass2srt;

class TestState2 : public statemachine::State<int> {
    public:
    virtual std::unique_ptr<statemachine::State<int>> transition(int &) override {
        return std::make_unique<statemachine::FinalState<int>>();
    }

    virtual void output(int &value) override {
        value += 10;
    }
};

class TestState1 : public statemachine::State<int> {
    public:
    virtual std::unique_ptr<statemachine::State<int>> transition(int &) override {
        return std::make_unique<TestState2>();
    }

    virtual void output(int &value) override {
        value++;
    }
};

TEST(statemachine, test_main_loop) {
    auto init_state = new TestState1;
    int init_value = 42;
    statemachine::run<int>(init_state, init_value);
    ASSERT_EQ(init_value, 53);
}
