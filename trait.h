#ifndef TRAIT_H
#define TRAIT_H

class Trait
{
    public:
        Trait(
            float initialValue,
            float deltaTick,
            float deltaSuccess,
            float lowThreshold
        );

        void UpdateTick();
        void CompleteTask();

        bool IsLow() const;
        float GetValue() const;

    protected:
        float value;
        float deltaTick;
        float deltaSuccess;
        float lowThreshold;
};

#endif

