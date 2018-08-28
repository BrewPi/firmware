#pragma once

#if 0
template <typename T,
          typename... Visitors>
class Visitable : public ObjectBase<T> {
public:
    ObjectBase() = default;
    virtual ~ObjectBase() = default;

    template <typename U,
              typename = std::enable_if_t<std::is_same<T, U>::value>>
    obj_type_t typeIdImpl(void) const
    {
        return resolveTypeId<U>();
    }

    /**
     * The application defined typeID for this object instance. Defined by derived class
     */
    virtual obj_type_t typeId() const override final
    {
        return typeIdImpl<T>();
    }

    template <typename U,
              typename V,
              typename = std::enable_if_t<std::is_same<T, U>::value>>
    void acceptImpl(V& visitor)
    {
        visitor.visit<U>(static_cast<U*>(this));
    }

    void accept(Visitors... visitor)
    {
        acceptImpl<T>(visitor);
    }
};
#endif
