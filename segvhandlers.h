#include <stdexcept>

inline void handle_segv()
{
    throw std::runtime_error("Segv error!");
}
inline void handle_fpe()
{
    throw std::runtime_error("Fpe error!");
}
