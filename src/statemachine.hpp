#ifndef _ASS2SRT_STATEMACHINE_H
#define _ASS2SRT_STATEMACHINE_H

#include <memory>

namespace ass2srt::statemachine {
    /**
     * Abstract state node
     */
    template<typename T>
    class State
    {
        public:
        /**
         * Get the next state
         */
        virtual std::unique_ptr<State<T>> transition(T &) = 0;

        /**
         * Calculate the output
         */
        virtual void output(T &) = 0;

        /**
         * Is ti a final node or not
         */
        inline virtual bool final() const
        {
            return false;
        }
    };

    /**
     * The final node
     */
    template<typename T>
    class FinalState : public State<T>
    {
        public:
        virtual std::unique_ptr<State<T>> transition(T &) override
        {
            return std::make_unique<FinalState<T>>();
        };

        virtual void output(T &) override
        {
        }

        inline virtual bool final() const override
        {
            return true;
        }
    };

    /**
     * Run the main loop
     */
    template<typename T>
    void run(State<T> *state, T &value)
    {
        auto curr_state = std::unique_ptr<State<T>>(state);
        while (!curr_state->final()) {
            curr_state->output(value);
            curr_state = curr_state->transition(value);
        }
    }
}

#endif
