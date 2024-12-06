#ifndef _ASS2SRT_STATEMACHINE_H
#define _ASS2SRT_STATEMACHINE_H

#include <memory>
#include <stdint.h>

namespace ass2srt { namespace statemachine {
    /**
     * Abstract state node
     */
    template<typename T>
    class State
    {
        public:
        /**
         * Unique id of the state
         */
        static const uint8_t id;

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
        static const uint8_t id = UINT8_MAX;

        virtual std::unique_ptr<State<T>> transition(T &value)
        {
            return std::make_unique<FinalState<T>>();
        };

        virtual void output(T &value)
        {
        }

        inline virtual bool final() const
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
} }

#endif
