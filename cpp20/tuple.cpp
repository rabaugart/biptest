#include <tuple>
#include <functional>
#include <iostream>
#include <cstddef>

template<typename S, size_t N, typename ... Args> struct finder_helper;

template<typename S, size_t N, typename H, typename ...Args> struct finder_helper<S,N,H,Args...> {
     static constexpr size_t value = std::conditional<std::is_same<S,H>::value,
     std::integral_constant<size_t,N>,finder_helper<S,N+1,Args...>>::type::value;
};

template<typename S, typename ...Args> struct finder;
template<typename S, typename ...Args> struct finder<S,std::tuple<Args...>> {
    static constexpr size_t value = finder_helper<S,0,Args...>::value;
};

struct frame_base {
    virtual std::string description() const = 0;
};

template<typename T> struct frame : public frame_base {
    T value;
    std::string name;
    frame() : value(), name(typeid(T).name()) {}
    std::string description() const {
        return name+"/"+std::to_string(value);
    }
};

std::ostream& operator<<( std::ostream& os, const frame_base& f ) {
    os << f.description();
    return os;
}

template<typename ...Args> struct frame_maker;
template<typename ...Args> struct frame_maker<std::tuple<Args...>> {
    using type = std::tuple<frame<Args>...>;
};

template<typename ...Args>
struct multi {
    using tuple_type = std::tuple<Args...>;
    using frame_tuple_type = typename frame_maker<tuple_type>::type;
    static constexpr size_t NARGS=sizeof...(Args);

    template<size_t I> std::string namei() const { return std::get<I>(frames).name; }

    template<typename T>
    struct indexer {
        static constexpr size_t value = finder<T,tuple_type>::value;
    };

    template<typename T> frame<T>& gett() { return std::get<indexer<T>::value>(frames); }

    template<typename T> std::string name() { return gett<T>().name; }

    template<size_t I> 
    const typename std::enable_if<I<NARGS,frame_base>::type& rgeti_helper( size_t i ) const {
        if ( I==i ) {
            return std::get<I>(frames);
        } else {
            return rgeti_helper<I+1>(i);
        }
    }

    template<size_t I> 
    const typename std::enable_if<I>=NARGS,frame_base>::type& rgeti_helper( size_t i ) const {
        throw std::runtime_error("Invalid i");
    }

    const frame_base& rgeti( size_t i ) const {
        return rgeti_helper<0>(i);
    }

    frame_tuple_type frames;
};

int main() {
    using my_tuple = std::tuple<int,long>;
    using my_frame_tuple = frame_maker<my_tuple>::type;
    multi<int,long,unsigned> m;
    std::cout << finder<frame<long>,my_frame_tuple>::value << " " << m.name<long>() << m.rgeti(2);
}
