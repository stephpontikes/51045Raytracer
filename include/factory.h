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
template <template <class, class> class M, typename Material, typename Geometry>
struct mesh_creator {
    // inherited methods are not namespaced if they are not called from a specific type;
    // smart to make multiple inheritance "overloads" virtual, or to use "using" statements to bring
    // them into the derived class namespace
    virtual unique_ptr<M<Geometry, Material>> doCreate(TT<Geometry> &&, TT<Material> &&, Vector3<double> const& color) {
        return make_unique<M<Geometry, Material>>(color);
    }
};

template <template <class, class> class T, typename G, typename M, typename... Ts>
struct mesh_creator<T, M, G(Ts...)> {
    // inherited methods are not namespaced if they are not called from a specific type;
    // smart to make multiple inheritance "overloads" virtual, or to use "using" statements to bring
    // them into the derived class namespace
    virtual unique_ptr<T<G, M>> doCreate(TT<G> &&, TT<M> &&, Vector3<double> const& color, Ts&&... ts) {
        // figure out how to get proper constructor call
        return make_unique<T<G, M>>(color, std::forward<Ts>(ts)...);
    }
};

template <template <class, class> typename T, typename M, typename... Ts>
struct parallel_mesh_factory : public mesh_creator<T, M, Ts>... {
    using mesh_creator<T, M, Ts>::doCreate...;
    template <typename G, typename... Us>
    std::unique_ptr<T<G, M>> create(Vector3<double> const& color, Us&&... us) {
        return this->doCreate(TT<G>(), TT<M>(), color, std::forward<Us>(us)...);
    }
};


}  // namespace mpcs51045

#endif
