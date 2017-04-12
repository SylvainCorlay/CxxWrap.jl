#ifndef XCONTAINER_JULIA_HPP
#define XCONTAINER_JULIA_HPP

#include <cstddef>
#include <array>
#include <algorithm>
#include <exception>

#include "xtensor/xcontainer.hpp"

namespace xt
{

    /***************************
     * jlcontainer declaration *
     ***************************/

    template <class D>
    class jlcontainer : public xcontainer<D>
    {
    public:

        using derived_type = D;

        using base_type = xcontainer<D>;
        using inner_types = xcontainer_inner_types<D>;
        using container_type = typename inner_types::container_type;
        using value_type = typename container_type::value_type;
        using reference = typename container_type::reference;
        using const_reference = typename container_type::const_reference;
        using pointer = typename container_type::pointer;
        using const_pointer = typename container_type::const_pointer;
        using size_type = typename container_type::size_type;
        using difference_type = typename container_type::difference_type;

        using shape_type = typename inner_types::shape_type;
        using strides_type = typename inner_types::strides_type;
        using backstrides_type = typename inner_types::backstrides_type;
        using inner_shape_type = typename inner_types::inner_shape_type;
        using inner_strides_type = typename inner_types::inner_strides_type;

        using iterable_base = xiterable<D>;

        using iterator = typename iterable_base::iterator;
        using const_iterator = typename iterable_base::const_iterator;

        using stepper = typename iterable_base::stepper;
        using const_stepper = typename iterable_base::const_stepper;

        using broadcast_iterator = typename iterable_base::broadcast_iterator;
        using const_broadcast_iterator = typename iterable_base::broadcast_iterator;

        void reshape(const shape_type& shape);

        using base_type::operator();
        using base_type::operator[];
        using base_type::begin;
        using base_type::end;
 
        jl_array_t* wrapped() noexcept;
 
    protected:

        jlcontainer() = default;
        ~jlcontainer() = default;

        jlcontainer(const jlcontainer&) = default;
        jlcontainer& operator=(const jlcontainer&) = default;

        jlcontainer(jlcontainer&&) = default;
        jlcontainer& operator=(jlcontainer&&) = default;
        jlcontainer(jl_array_t*) noexcept;

        jl_array_t* p_array;
    };

    /******************************
     * jlcontainer implementation *
     ******************************/

    /**
     * Reshapes the container.
     * @param shape the new shape
     */
    template <class D>
    inline void jlcontainer<D>::reshape(const shape_type& shape)
    {
        if (shape.size() != this->dimension() || !std::equal(shape.begin(), shape.end(), this->shape().begin()))
        {
            strides_type strides = make_sequence<strides_type>(shape.size(), size_type(1));
            compute_strides(shape, layout::column_major, strides);
            reshape(shape, strides);
        }
    }

    /**
     * Return the wrapped julia array.
     */
    template <class D>
    inline auto jlcontainer<D>::wrapped() noexcept -> jl_array_t*
    {
        return p_array;
    }

    /**
     * Constructor from julia
     * @param ptr the julia array
     */
    template <class D>
    inline jlcontainer<D>::jlcontainer(jl_array_t* ptr) noexcept
        : p_array(ptr)
    {
    }
}

#endif

