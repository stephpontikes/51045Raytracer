#ifndef FACTORY_H
#define FACTORY_H
#include <memory>
#include <tuple>
using std::make_unique;
using std::tuple;
using std::unique_ptr;


namespace mpcs51045 {
template <typename T>
struct TT {
};

template <typename T>
struct abstract_creator {
    virtual unique_ptr<T> doCreate(TT<T> &&) = 0;
};

template <typename... Ts>
struct abstract_factory : public abstract_creator<Ts>... {
    template <class U>
    unique_ptr<U> create() {
        abstract_creator<U> &creator = *this;
        return creator.doCreate(TT<U>());
    }
    virtual ~abstract_factory() = default;
};

template <typename AbstractFactory, typename Abstract, typename Concrete>
struct concrete_creator : virtual public AbstractFactory {
    unique_ptr<Abstract> doCreate(TT<Abstract> &&) override {
        return make_unique<Concrete>();
    }
};

template <typename AbstractFactory, typename... ConcreteTypes>
struct concrete_factory;

template <typename... AbstractTypes, typename... ConcreteTypes>
struct concrete_factory<abstract_factory<AbstractTypes...>, ConcreteTypes...>
    : public concrete_creator<abstract_factory<AbstractTypes...>,
                              AbstractTypes, ConcreteTypes>... {
};

// todo: arguments for mesh factory types
template <template <class, class> class M, typename Geometry, typename Material>
struct mesh_creator {
    // inherited methods are not namespaced if they are not called from a specific type;
    // smart to make multiple inheritance "overloads" virtual, or to use "using" statements to bring
    // them into the derived class namespace
    virtual unique_ptr<M<Geometry, Material>> doCreate(TT<Geometry> &&, TT<Material> &&) {
        return make_unique<M<Geometry, Material>>();
    }
};

template <template <class, class> typename M, typename Geometries, typename Materials>
struct parallel_mesh_factory;

template <template <class, class> typename M, typename... Ts, typename U>
struct parallel_mesh_factory<M, tuple<Ts...>, U>
    : public mesh_creator<M, Ts, U>... {
    template <typename Geometry>
    std::unique_ptr<M<Geometry, U>> create() {
        return this->doCreate(TT<Geometry>(), TT<U>());
    }
};

}  // namespace mpcs51045

#endif
