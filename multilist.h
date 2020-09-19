#ifndef MULTILIST_H
#define MULTILIST_H

#include <utility>
#include <set>

#include "randutils.h"

template <class T>
class MultiElement
{
    public:
        template<class... Args>
        MultiElement<T>(Args&&... args):
            element(std::forward<Args>(args)...)
        {
        }

        void AddNeighbor(MultiElement<T> *other)
        {
            neighbors.insert(other);
        }

        bool RemoveNeighbor(MultiElement<T> *neighbor)
        {
            return neighbors.erase(neighbor) != 0;
        }

        MultiElement<T> *GetRandomNeighbor()
        {
            auto it = getRandom(neighbors);
            if(it == neighbors.end())
                return nullptr;
            return *it;
        }

        std::set<MultiElement<T>*> GetRandomNeighbors(size_t n)
        {
            if(n == neighbors.size())
                return neighbors;

            std::set<MultiElement<T>*> copy(neighbors);
            std::set<MultiElement<T>*> ret;
            while(n-- && copy.size() > 0)
            {
                auto it = getRandom(copy);
                if(it == copy.end())
                    break;
                ret.insert(*it);
                copy.erase(it);
            }
            return ret;
        }

        const T &GetElement() const
        {
            return element;
        }

        const std::set<MultiElement<T>*> &GetNeighbors() const
        {
            return neighbors;
        }

    private:
        static std::set<MultiElement<T>*>::iterator getRandom(std::set<MultiElement<T>*> &s)
        {
            if(s.size() == 0)
                return s.end();
            size_t index = randutils::randui(0, s.size()-1);
            auto it = s.begin();
            while(index--)
                it++;
            return it;
        }

        T element;
        std::set<MultiElement<T>*> neighbors;
};

template <class T>
class MultiList
{
    public:
        MultiList()
        {
        }

        ~MultiList()
        {
            for(auto it: elements)
                delete it;
        }

        template<class... Args>
        MultiElement<T> *AddElement(Args&&... args)
        {
            MultiElement<T> *element = new MultiElement<T>(args...);
            elements.insert(element);
            return element;
        }

        static void LinkElements(MultiElement<T> *a, MultiElement<T> *b)
        {
            a->AddNeighbor(b);
            b->AddNeighbor(a);
        }

        void RemoveElement(MultiElement<T> *element)
        {
            for(auto it : elements)
                it->RemoveNeighbor(element);
            elements.erase(element);
            delete element;
        }

        MultiElement<T> *GetRandomElement()
        {
            size_t index = randutils::randui(0, elements.size()-1);
            auto it = elements.begin();
            while(index--)
                it++;
            return *it;
        }

    private:
        std::set<MultiElement<T>*> elements;
};

#endif

