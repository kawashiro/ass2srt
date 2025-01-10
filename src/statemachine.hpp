#ifndef ASS2SRT_STATEMACHINE_H
#define ASS2SRT_STATEMACHINE_H

#include <memory>

namespace ass2srt::statemachine {
/**
 * Abstract state node
 */
template <typename T>
class State {
public:
    /**
     * Get the next state
     */
    virtual auto transition(T&) -> std::unique_ptr<State<T>> = 0;

    /**
     * Calculate the output
     */
    virtual void output(T&) = 0;

    /**
     * Is ti a final node or not
     */
    virtual auto final() -> bool
    {
        return false;
    }

    State() = default;
    State(const State<T>&) = default;
    State(State<T>&&) = default;

    auto operator=(const State<T>&) -> State<T>& = default;
    auto operator=(State<T>&&) -> State<T>& = default;

    virtual ~State() = default;
};

/**
 * The final node
 */
template <typename T>
class FinalState : public State<T> {
public:
    auto transition(T&) -> std::unique_ptr<State<T>> override
    {
        return std::make_unique<FinalState<T>>();
    };

    void output(T&) override
    {
    }

    auto final() -> bool override
    {
        return true;
    }
};

/**
 * Run the main loop
 */
template <typename T>
void run(State<T>* state, T& value)
{
    auto curr_state = std::unique_ptr<State<T>>(state);
    while (!curr_state->final()) {
        curr_state->output(value);
        curr_state = curr_state->transition(value);
    }
}
}

#endif
